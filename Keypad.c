#include "tm4c123gh6pm.h"
#include "systic.h"
#include "keypad.h"
#define readsw1       (GPIO_PORTF_DATA_R & 0x10)
#define readsw2       (GPIO_PORTF_DATA_R & 0x01)


void KEYPAD_INIT(void){
	
	SYSCTL_RCGCGPIO_R |= 0x05;
	while((SYSCTL_PRGPIO_R & 0x05) == 0){};
		GPIO_PORTC_LOCK_R = 0x4C4F434B;
		GPIO_PORTC_CR_R |= 0xF0;
		GPIO_PORTC_AMSEL_R &= ~0xF0;
		GPIO_PORTC_PCTL_R &= ~0xFFFF0000;
		GPIO_PORTC_AFSEL_R &= ~0xF0;
		GPIO_PORTC_DIR_R |= 0xF0;   // to be updated
		GPIO_PORTC_DEN_R |= 0xF0;
		
		GPIO_PORTA_LOCK_R = 0x4C4F434B;
		GPIO_PORTA_CR_R |= 0xFF;
		GPIO_PORTA_AMSEL_R &= ~0xFF;
		GPIO_PORTA_PCTL_R &= ~0xFFFFFFFF;
		GPIO_PORTA_AFSEL_R &= ~0xFF;
		GPIO_PORTA_DIR_R  = 0x07;   // to be updated
		GPIO_PORTA_DEN_R |= 0xFF;
		GPIO_PORTA_PUR_R |= 0xF8;
		GPIO_PORTA_DATA_R &= ~(0x04);
	}
unsigned char KEYPAD_READ(){
	
		unsigned char arr[4][4] = {{'1','2','3','A'},
															{'4','5','6','B'},
															{'7','8','9','C'},
															{'*','0','#','D'}};
	
		char row, col, x;
		//unsigned char return_value = 0xFF;
while(1){	
		for(row = 0; row < 4; row++)
		{
			GPIO_PORTC_DATA_R = 0xF0;
			GPIO_PORTC_DATA_R &= ~(1 << (row+4));
			delay_ms(1);
		
			for(col = 0; col < 4; col++)
			{
			/*	door = ((GPIO_PORTA_DATA_R & 0x08) >> 3);
				if(door != 0) LCD_COMMAND(1); //switch ta7t*/
				x = (GPIO_PORTA_DATA_R & (1 << (col+4)));
				if(readsw1 == 0 ) return 0;
				if(readsw2 == 0 ) return 1;
				if(x == 0)
				{
				
					return( arr[row][col]);
					
				}
			}
		}
}

}
