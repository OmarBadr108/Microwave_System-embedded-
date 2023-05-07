#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "LCD.h"
#include "systic.h"


void LCD_COMMAND ( char com )
		{
      GPIO_PORTE_DATA_R &= ~ 0x06; //
			//Rs=0 Rw=0
			GPIO_PORTE_DATA_R |= 0x08;
			GPIO_PORTB_DATA_R = com;
      delay_ms(5);
		  GPIO_PORTE_DATA_R &=~0x08;
			delay_ms(15);
		}
		
void LCD_initialization ()
	{

	
			SYSCTL_RCGCGPIO_R |= 0X12;
	while((SYSCTL_PRGPIO_R & 0x12) == 0){};
		//PORTB
		GPIO_PORTB_LOCK_R = 0x4C4F434B;
		GPIO_PORTB_CR_R |= 0xFF;
		GPIO_PORTB_AMSEL_R &= ~0xFF;
		GPIO_PORTB_PCTL_R &= ~0xFFFFFFFF;
		GPIO_PORTB_AFSEL_R &= ~0xFF;
		//PORTE
		GPIO_PORTB_LOCK_R = 0x4C4F434B;
		GPIO_PORTE_CR_R |= 0x0E;
		GPIO_PORTE_AMSEL_R &= ~0x0E;
		GPIO_PORTE_PCTL_R &= ~0x0000FFF0;
		GPIO_PORTE_AFSEL_R &= ~0x0E;
		
		GPIO_PORTB_DIR_R = 0xFF;
		GPIO_PORTB_DEN_R = 0xFF;
		GPIO_PORTE_DIR_R |= 0x0E;
		GPIO_PORTE_DEN_R |= 0x0E;
			
		
		//////
			delay_ms(30);
		delay_ms(2);
		LCD_COMMAND (0X30);
		delay_ms(5);
		LCD_COMMAND (0X38);
		delay_ms(15);
		LCD_COMMAND (0X0E);
		delay_ms(15);
		LCD_COMMAND (0X02);
		delay_ms(15);
		LCD_COMMAND (0X06);
		delay_ms(15);
	}
	
	
		
		
		void LCD_DATA (char data){
		GPIO_PORTE_DATA_R &= ~0x04;//Rw=0
		GPIO_PORTE_DATA_R |= 0x02;//Rs=1
		GPIO_PORTE_DATA_R |= 0x08;//enable = 1
		GPIO_PORTB_DATA_R = data;
		delay_ms(20);
		GPIO_PORTE_DATA_R &= ~0x08;
		delay_ms(20);
		
		
	}
		
	void LCD_STRING(char *String)
{
	while(*String)// will stop when it reaches null terminator
	{
		LCD_DATA(*String);
		String++;
	}


}





