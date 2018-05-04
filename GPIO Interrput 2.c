#include "stdint.h"
#include "tm4c123gh6pm.h"
void SystemInit() {}
uint32_t counter=0;
void systick_init()
{
	NVIC_ST_CTRL_R=~1;
	NVIC_ST_RELOAD_R=0xFFFFFF;
	NVIC_ST_CURRENT_R=0;
	NVIC_ST_CTRL_R=7;
}	
void portf_init()
{	
	uint32_t dummy;
	SYSCTL_RCGCGPIO_R |= 1<<5;
	dummy=0;
	dummy=1;
	GPIO_PORTF_LOCK_R=0x4c4f434b;
	GPIO_PORTF_CR_R=0XFF;
	
	//CONFIGURE PUSHBUTTON
	GPIO_PORTF_DIR_R |=(1<<2);
	GPIO_PORTF_DIR_R &=~(1<<0);
	GPIO_PORTF_DEN_R |=(1<<0) | (1<<2);
	GPIO_PORTF_PUR_R |=1<<0;
	
	
	//CONFIGURE INTERRUPT PIN
	GPIO_PORTF_IS_R &=~(1<<0);
	GPIO_PORTF_IBE_R &=~(1<<0);
	GPIO_PORTF_IEV_R &=~(1<<0);
	GPIO_PORTF_ICR_R |=1<<0;
	GPIO_PORTF_IM_R |=1<<0;
	
	//CONFIGURE INTERRPUT REQUEST
	NVIC_EN0_R |=1<<30;
}

void seven_segments_init()
{
		uint32_t dummy;
		SYSCTL_RCGCGPIO_R |= (1<<1) | (1<<3) |(1<<4);  												 // Enable port B,D,E
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
}

void GPIOF_Handler()
{
	uint32_t temp;
	temp=counter;
	GPIO_PORTF_ICR_R |=1<<0;
	GPIO_PORTF_DATA_R ^=1<<2;
	GPIO_PORTB_DATA_R = (uint32_t)(counter%10);//multiple of 1
	counter /= 10;
	GPIO_PORTD_DATA_R =	(uint32_t)(counter%10);
	counter /= 10;
	GPIO_PORTE_DATA_R =	((uint32_t)(counter%10))<<1;
	counter=temp;
	counter++;
}
	
int main()
{
	portf_init();
	seven_segments_init();
	__enable_irq();
	GPIO_PORTB_DATA_R=0;
	GPIO_PORTD_DATA_R=0;
	GPIO_PORTE_DATA_R=0<<1;
	GPIO_PORTF_DATA_R |=1<<2;
	while(1)
	{
		
	}
	return 0;
}