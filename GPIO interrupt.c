#include "stdint.h"
#include "tm4c123gh6pm.h"
void SystemInit() {}

void systick_init()
{
	NVIC_ST_CTRL_R=~1;
	NVIC_ST_RELOAD_R=0xFFFFFF;
	NVIC_ST_CURRENT_R=0;
	NVIC_ST_CTRL_R=7;
}	
void portf_init()
{
	SYSCTL_RCGCGPIO_R |=1<<5;
	GPIO_PORTF_LOCK_R=0x4c4f434b;
	GPIO_PORTF_CR_R=0xFF;
	GPIO_PORTF_DIR_R |=(1<<1) | (1<<2) | (1<<3);
	GPIO_PORTF_DIR_R &=~((1<<0) | (1<<4));
	GPIO_PORTF_DEN_R |=(1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4);
	GPIO_PORTF_PUR_R |=1<<4;
	
	//INTERRUPT
	GPIO_PORTF_IS_R |=(1<<0);
	GPIO_PORTF_IBE_R &=~(1<<0);
	GPIO_PORTF_IEV_R |=(1<<0);
	GPIO_PORTF_ICR_R |=(1<<0); //CLEAR FLAF
	GPIO_PORTF_IM_R |=(1<<0);
	NVIC_EN0_R |=(1<<30);
}
void GPIOF_Handler()
{
	GPIO_PORTF_ICR_R |=(1<<0);
	GPIO_PORTF_DATA_R ^=1<<1;
}
	

int main()
{
	//systick_init();
	portf_init();
	//GPIO_PORTF_DATA_R=1<<1;
	__enable_irq();
	while(1)
	{
		
	}
	return 0;
}