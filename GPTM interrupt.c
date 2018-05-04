#include "stdint.h"
#include "tm4c123gh6pm.h"
void SystemInit() {}

void GPIO_init()
{
		int dummy;
		SYSCTL_RCGCGPIO_R |= (1<<1);  												 // Enable port B
		dummy=0;
		dummy=1;
		//portB 0,1,2,3 for units display
		GPIO_PORTB_LOCK_R = 0x4c4f434b;
		GPIO_PORTB_CR_R=0xFF;
		GPIO_PORTB_DIR_R |= 1 | (1<<1) | (1<<2) | (1<<3);
		GPIO_PORTB_DEN_R |= 1 | (1<<1) | (1<<2) | (1<<3);
}
void timer_init()
{
	int dummy;
	SYSCTL_RCGCTIMER_R |=0x01;
	dummy=0;
	dummy=1;
	TIMER0_CTL_R=0; //disable timer
	TIMER0_CFG_R=0; //mode is 32 bit
	TIMER0_TAMR_R=2; //periodic mode
	TIMER0_TAILR_R=160000-1;
	TIMER0_TAPR_R=0;
	TIMER0_ICR_R=1;
	TIMER0_IMR_R=1;
	NVIC_EN0_R |=1<<19;
	TIMER0_CTL_R =0x000000001;
}
void TIMER0A_HANDLER()
{
	TIMER0_ICR_R=1;
	GPIO_PORTB_DATA_R=9;
}
int main()
{
	GPIO_init();
	GPIO_PORTB_DATA_R=0;
	__enable_irq();
	timer_init();
	while(1)
	{
		
	}
	return 0;
}