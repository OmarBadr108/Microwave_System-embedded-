#include "tm4c123gh6pm.h"
#include "lcdmacros.h"
#include "LCD.h"
#include "systic.h"
#include "keypad.h"
#define readsw1       (GPIO_PORTF_DATA_R & 0x10)
#define readsw2       (GPIO_PORTF_DATA_R & 0x01)
#define readDoor  		(GPIO_PORTA_DATA_R & 0x08)
#define Buzzer				 (GPIO_PORTA_DATA_R)   //pin A2 (0100)
#define ctoi(c)		(c - '0')
#define itoc(i)		(i + '0')


void SystemInit(void);
void lcdTimer(int timer);
void Finish(void);
void cooking(char* dish ,int timer);
char pause(char check);
int defrosting(char *dish , int weightFactor);
int customTimer(void);


char *dish,input,sw1,sw2,door;
int timer;


int main(){

	  SysTick_Init();
    LCD_initialization ();
	  KEYPAD_INIT();
    SystemInit();
    LCD_COMMAND (0X01);

		timer = 0;
	while(1){
			sw1 =1; sw2=1 ; door = 0;
			SystemInit();
			LCD_COMMAND (clear);
			LCD_STRING("Please, choose");
			LCD_COMMAND (SecondRow);
			LCD_STRING( "from A,B,C,D:" );
			input = KEYPAD_READ();		//read from keypad a,b,c,d
			
			switch(input){
					case 'A':	
							LCD_DATA(input);
							delay_ms(1000);
							LCD_COMMAND(1);
							LCD_STRING ("Close door &");
							LCD_COMMAND(SecondRow);
							LCD_STRING ("press sw2");
							dish = "POPCORN" ;
							while( (sw2 != 0) || (door == 0) ){
							sw2 = readsw2;
							door = readDoor;
							}
							cooking(dish ,60); 

							break;

					case 'B':
							LCD_DATA(input );
							delay_ms(1000);
							dish = "Beef";
							timer = defrosting(dish,30);
							LCD_COMMAND(0x01);
							LCD_STRING ("Close door &");
							LCD_COMMAND(SecondRow);
							LCD_STRING ("press sw2");					
							while( (sw2 != 0) || (door == 0) ){   //if door is closed and switch 2 is pressed begin cooking
									sw2 = readsw2;
									door = readDoor;
									}
							cooking(dish,timer);
							break;

					case 'C':
							LCD_DATA(input );
							delay_ms(1000);
							dish = "chicken";
							timer = defrosting(dish,12);
							LCD_COMMAND(0x01);
							LCD_STRING ("Close door &");
							LCD_COMMAND(SecondRow);
							LCD_STRING ("press sw2");					
							while( (sw2 != 0) || (door == 0) ){   //if door is closed and switch 2 is pressed begin cooking
									sw2 = readsw2;
									door = readDoor;
									}
							cooking(dish,timer);
							break;
	
					case 'D':	
											
											 LCD_DATA(input );
											 delay_ms(1000);
											 timer = customTimer();
											 LCD_COMMAND(1);
											 LCD_STRING ("Close door &");
											 LCD_COMMAND(SecondRow);
											 LCD_STRING ("press sw2");
												while( (sw2 != 0) || (door == 0) ){
													sw2 = readsw2;
													door = readDoor;
												}
										   cooking("cooking time:" , timer );


						break;
	
					default:
					{
							LCD_COMMAND(clear);
							dish = "invalid input";
							LCD_STRING(dish);
							delay_ms(1000);
					}

	GPIO_PORTF_DATA_R &= ~(0x0E); //leds off when stopped;
	delay_ms(50);
}

}




//case B and C function to accept weight and print corresponding cooking time and represents cooking state
int defrosting(char *dish , int weightFactor){
			char weight = 0 ;
			LCD_COMMAND(0x01);
BADR: 			LCD_STRING(dish);
			LCD_STRING(" weight?");
			
			weight = KEYPAD_READ() ;
			if( weight < '1' || weight > '9' ){		//if user entered wrong input print err for 1 sec 
			LCD_COMMAND(0x01);
			LCD_STRING ("Err");
			delay_ms(1000);
			LCD_COMMAND(0x01);
				goto BADR;
			}
			LCD_COMMAND(0x01);
			LCD_STRING(dish);		//dish name at first line
			LCD_COMMAND(SecondRow);
			LCD_STRING("weight is:");
			LCD_DATA(weight);
			LCD_STRING("kg");		//entered weight in kg at second line
			delay_ms(2000);
			weight = weight - '0';  //converting weight to integer to calculate time
			return (weightFactor*weight);
}





void cooking(char* dish ,int timer){   //cooking state
	char reset = 0;
	char i ;
	LCD_COMMAND(clear);
	LCD_STRING(dish);
	while(timer >= 0){
			GPIO_PORTF_DATA_R |= 0x0E; //leds are on;
			door = readDoor;	
			sw1 = readsw1;
			lcdTimer(timer);
			if (timer!= 0 ){
				for(i=0 ; i<39 ; i++){
						sw1 = readsw1 ;
						sw2 = readsw2 ;
						door = readDoor ;
						delay_ms(20);							//if door is open door = 1) or sw1 is pressed (sw1=0) pause		
								if( door == 0 ) reset = pause(sw1) ;
								if(sw1 == 0) {			//solving debouncing problem
									delay_ms(20);
									sw1 = readsw1 ;
									if(sw1 == 0 ) reset = pause(sw1) ;
								}
								if (reset == 1) return;
																		
				
			}
		}
			timer--;

		}
	Finish();
}	
		
		
		
		
		
		// system initilization 
		void SystemInit(void){

SYSCTL_RCGCGPIO_R |= 0x20; // PortF clock enable
while ((SYSCTL_PRGPIO_R & 0x20)==0); //Delay
GPIO_PORTF_LOCK_R = 0x4C4F434B; // Unlock PortF Commit register
GPIO_PORTF_CR_R |= 0x1F; // Allow changes to PF
GPIO_PORTF_AMSEL_R &= ~(0x1F); // Disable analog function
GPIO_PORTF_PCTL_R &= ~0x000FFFFF; // GPIO clear bit PCTL
GPIO_PORTF_AFSEL_R &= ~(0x1F); // No alternate function
GPIO_PORTF_DIR_R |= 0x0E; // PF321 output
GPIO_PORTF_DEN_R |= 0x1F; // Enable digital pins PF4-PF0
GPIO_PORTF_PUR_R |= 0x11;	
GPIO_PORTF_DATA_R &= ~(0x0E); // Initialize LEDs to be off
}

		//function to print time on LCD in minutes and seconds

void lcdTimer(int timer){

	int min,sec;
	char display[6];
	min = timer/60 ; sec = timer%60;
	*(display+0) = '0' + (min/10) ; //display digits from left to right & converting to char
	*(display+1) = '0' + ( min%10);
	*(display+2) = ':';
	*(display+3) = '0' + (sec/10) ;
	*(display+4) = '0' + ( sec%10);
	*(display+5) = '\0';
	LCD_COMMAND(SecondRow);
	LCD_STRING(display);
}



	
/// function to blink when cooking is done

void Finish(){  //finish state
		int i = 0 ;
		LCD_COMMAND(clear);
		LCD_STRING("HAPPY MEAL");
		while(i < 6){
		GPIO_PORTF_DATA_R ^= 0x0E;      //toggle Leds
		Buzzer ^= 0x04;			//toggle Leds and buzzer
		delay_ms (1000);
		i++;

	}

}
	
}	

}	
// puase function when sw1 is clicked once or the is open  reprents pause state
char pause(char check1){
		char reset = 0;
		char i,check2;
		check2 =1;

		while(1)		//loop that will halt the operation till door is closed sw2 is pressed or sw1 is pressed
		{	
			GPIO_PORTF_DATA_R ^= 0x0E; //leds are toggled;
			for(i=0 ; i<50 ; i++){ // cheacks the switches every 20 ms 
			sw1 = readsw1 ;
			sw2 = readsw2 ;
			door = readDoor ;
			delay_ms(20);
			
			if( (door!=0) && (sw2 == 0) ) return reset; // reset if the door is open and sw1 is pressed 
			if(sw1 == 0) {
									delay_ms(100);
									sw1 = readsw1 ;
									if(sw1 == 0 ) {
											check2 = 0 ;
											if(check1 == 0) return 1;
											if(check2 == 0) return 1;
											//check2 = sw1;
												
								}
							}
				
		}
			
	}
		

}

int customTimer(){    //input state to take input in case D
ERR:	LCD_COMMAND(1);
			LCD_STRING ("Cooking time? ");
			delay_ms(1000);
			LCD_COMMAND(1);
			char display[6];     //initialize display with "00:00"
			*(display+0) = '0' ;
			*(display+1) = '0';
			*(display+2) = ':';
			*(display+3) = '0' ;
			*(display+4) = '0';
			*(display+5) = '\0';
			char check ;
			int i,min,sec,timer;
			i = 4 ;
			LCD_COMMAND(1);
				
		while(i>=0 ){				
		check = KEYPAD_READ();
		if(check == 1)	break;		//if sw2 is pressed exit	
		if(check == 0) goto ERR;	//if sw1 is pressed reset
		LCD_COMMAND(1);
		display[0] = display[1];
    		display[1] = display[3];
    		display[3] = display[4];
    		display[4] = check;		//shifting the input time where the new value is entered from the right
		LCD_STRING(display);
		LCD_COMMAND(SecondRow);
		LCD_STRING("sw2 to enter");


			i--;
			
	}
		if( ( (display[0] >= '3') &&  (display[1] >= '5') ) || (display[3] >= '5') ) {
				LCD_COMMAND(1);
				LCD_STRING("Invalid time");
				delay_ms(1000);
				goto ERR;
			
				}


	min = ctoi(display[0])*10 + ctoi(display[1]);
	sec = ctoi(display[3])*10 + ctoi(display[4]);

	timer = min*60 + sec;
	if(i==0)delay_ms(1000);
	return(timer);


}


