#include "systic.h"
#include "tm4c123gh6pm.h"

void SysTick_Init(void){
		NVIC_ST_CTRL_R = 0;
		NVIC_ST_RELOAD_R = 0x00FFFFFF;
		NVIC_ST_CURRENT_R = 0;
		NVIC_ST_CTRL_R = 0x00000005;
}

void SysTick_Wait(unsigned int delay){
		NVIC_ST_RELOAD_R = delay - 1;
		NVIC_ST_CURRENT_R = 0;
		while((NVIC_ST_CTRL_R & 0x00010000)==0){};
}
void delay_ms(unsigned int delay){
		unsigned int i;
		for(i = 0;i<delay;i++){
			SysTick_Wait(80000/5);
		}
}
void delay_micro(unsigned int delay){
		unsigned int i;
		for(i = 0;i<delay;i++){
			SysTick_Wait(16);
		}
}
