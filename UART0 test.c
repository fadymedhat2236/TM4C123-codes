


#include "tm4c123gh6pm.h"
#include "stdint.h"
void SystemInit(){}

void clock_init(int clk)
{
    int SYSVID = (400/clk)-1;

    if(SYSVID<4) SYSVID=4;
    if(SYSVID>16) SYSVID=16;


    SYSCTL_RCC2_R |= (unsigned long)1<<31; // enable rcc2 to override rcc
    SYSCTL_RCC2_R |= 1<<11; //enable 16 mhz clock to make the system operatable at first
    SYSCTL_RCC_R = (SYSCTL_RCC_R & (~0x000007C0)) + 0x540; // put 0x15 at XTAL to adjust it to the 16 mhz crystal
    SYSCTL_RCC2_R &= ~(7<<4);
    SYSCTL_RCC2_R &= ~(1<<13);
    SYSCTL_RCC2_R |= 1<<30;
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R & (~0x1FC00000)) + (SYSVID<<22); //put value of XTAL
    while(SYSCTL_RIS_R & 1<<6); //wait for pll to be stable
    SYSCTL_RCC2_R &= ~(1<<11); //make the system use pll

}

	
void UART0_INIT()
{
	SYSCTL_RCGCGPIO_R |=1<<0;		//ENABLE CLOCK FOR PORTA
	SYSCTL_RCGCUART_R |=1<<0;  	//ENABLE CLOCK FOR UART0
	GPIO_PORTA_AFSEL_R=0x03;
	GPIO_PORTA_PCTL_R |=(1<<0) | (1<<4);
	GPIO_PORTA_DEN_R |=(1<<0) | (1<<1);
	UART0_CTL_R=~1;		//DISABLE UART0
	UART0_IBRD_R=104;
	UART0_FBRD_R=11;
	UART0_LCRH_R =(1<<5) | (1<<6);
	UART0_CTL_R =(1<<0) | (1<<8) | (1<<9);
	UART0_CC_R=0x00;
	//CONFIGURE LEDS
	SYSCTL_RCGCGPIO_R |=1<<5;
	GPIO_PORTF_LOCK_R=0x4c4f434b;
	GPIO_PORTF_CR_R=0xFF;
	GPIO_PORTF_DIR_R |=(1<<1) | (1<<2) | (1<<3);
	GPIO_PORTF_DEN_R |=(1<<1) | (1<<2) | (1<<3);
}

void printChar(char c)
{
	while((UART0_FR_R & (1<<5)) !=0);
	UART0_DR_R=c;
}

void printString(char* s)
{
	while(*s)
	{
		printChar(*(s++));
	}
}
char readChar()
{
	char c;
	while((UART0_FR_R & (1<<4)) !=0);
	c=UART0_DR_R;
	return c;
}
int main()
{
	UART0_INIT();
	printString("hello");
	printString("\n\r");
	while(1)
	{
		char c;
		printString("enter a character");
		printString("\n\r");
		c=readChar();
		printChar(c);
		printString("\n\r");
		if(c=='r')
			GPIO_PORTF_DATA_R=1<<1;
		else if(c=='b')
			GPIO_PORTF_DATA_R=1<<2;
		else if(c=='g')
			GPIO_PORTF_DATA_R=1<<3;
		else
			GPIO_PORTF_DATA_R=0;
	}
	return 0;
}
