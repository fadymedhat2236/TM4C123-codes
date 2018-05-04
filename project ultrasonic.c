#include "tm4c123gh6pm.h"
#include "stdint.h"
void SystemInit(){}

void wait(uint32_t delay)
{
	NVIC_ST_RELOAD_R=delay-1;
	NVIC_ST_CURRENT_R=0;
	while((NVIC_ST_CTRL_R &(1<<16))==0);
}
void wait_10msec(uint32_t delay)
{
	uint32_t i;
	for(i=0;i<delay;i++)
	wait(160000);

}


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

/*=========================== Initialize GPIO =====================*/

void GPIO_Init(void)
{
		int dummy;
	
		SYSCTL_RCGCGPIO_R = (1<<5) | (1<<1) |(1<<3) |(1<<4) | (1<<5);  												 // Enable port A,B,D,E
		dummy=0;
		dummy=1;
		
		//portB 0,1,2,3 for units display
		GPIO_PORTB_LOCK_R = 0x4c4f434b;
		GPIO_PORTB_CR_R=0xFF;
		GPIO_PORTB_DIR_R |= 1 | (1<<1) | (1<<2) | (1<<3);
		GPIO_PORTB_DEN_R |= 1 | (1<<1) | (1<<2) | (1<<3);
	

		//portD 0,1,2,3 for tens display
		GPIO_PORTD_LOCK_R = 0x4c4f434b;
		GPIO_PORTD_CR_R=0xFF;
		GPIO_PORTD_DIR_R |= 1 | (1<<1) | (1<<2) | (1<<3);
		GPIO_PORTD_DEN_R |= 1 | (1<<1) | (1<<2) | (1<<3);
	
	
		//portE 1,2,3,4 for hundreds display
		GPIO_PORTE_LOCK_R = 0x4c4f434b;
		GPIO_PORTE_CR_R=0xFF;
		GPIO_PORTE_DIR_R |= (1<<1)  | (1<<2) | (1<<3) | (1<<4);
		GPIO_PORTE_DEN_R |= (1<<1)  | (1<<2) | (1<<3) | (1<<4);
		
		//ultrasonic
		GPIO_PORTF_LOCK_R = 0x4c4f434b;
		GPIO_PORTF_CR_R=0xFF;
		GPIO_PORTD_DIR_R &= ~ ( (1<<7) ) ; 	
		GPIO_PORTD_DEN_R |=  (1<<7);
		GPIO_PORTF_DIR_R |=  (1<<2); 			
		GPIO_PORTF_DEN_R |=  (1<<2);   



		
		
}




/*============================= Initialize Timer ========================*/

void Systick_Init(void)
{
	NVIC_ST_CTRL_R = 0; 
	NVIC_ST_RELOAD_R = 0xffffff;  
	NVIC_ST_CURRENT_R = 0x000000;
	NVIC_ST_CTRL_R=0x05;
}



void get_distance(void)
{
	uint32_t time_now ;
	uint32_t distance ;
	
	/* Set Trigger pulse */
	GPIO_PORTF_DATA_R |= 1<<2 ; //output 1 to trigger
	wait(160);
	GPIO_PORTF_DATA_R &= ~(1<<2); //output 0 to trig, pulse eneded
	

	
	while( ((GPIO_PORTD_DATA_R & (1<<7))>>7)==0 )   //wait until echo goes from low to high, start the systick
	{
		
	} 
		
	NVIC_ST_RELOAD_R = 0xffffff;
	NVIC_ST_CURRENT_R=0;
	
	while( ((GPIO_PORTD_DATA_R & (1<<7))>>7)==1 )
	{
		
	}
	
	// calculate time.
	time_now = ( 0xFFFFFF- NVIC_ST_CURRENT_R ) ;
	distance=time_now/928;
	//3adad * time one cycle * 1M/58
	
	// Display distance.
	GPIO_PORTB_DATA_R = (uint32_t)(distance%10);//multiple of 1
	distance /= 10;
	GPIO_PORTD_DATA_R =	(uint32_t)(distance%10);
	distance /= 10;
	GPIO_PORTE_DATA_R =	((uint32_t)(distance%10))<<1;
	wait_10msec(100); 
}


int main()
{
	GPIO_Init();
	Systick_Init();
	
	while(1)
	{
		get_distance();
	}

	return 0;
}









