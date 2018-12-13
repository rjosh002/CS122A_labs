#include <avr/io.h>
#include "io.c"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#define tasksNum 1
#include "scheduler.h"
#define F_CPU 8000000UL
static volatile int pulse = 0;
static volatile int i = 0;


#define timerPeriod 1000
int16_t count_a = 0;
char show_a[16];

enum SM1_States {sm1_display} state;  //display state machine
int SMTick1(int state) {
	
	
	
	// === Local Variables ===

	
	// === Actions ===

	switch (state) {
		//unsigned char i=0;
		case sm1_display:   //populates character
// 		PORTD |= 1<<PIND0;
// 		_delay_us(15);
// 		
// 		PORTD &= ~(1<<PIND0);
// 		count_a = pulse/58;
		
		
		//delay_ms(100);
		//	LCD_DisplayString(1,"Distance=");

		itoa(count_a,show_a,10);
				LCD_DisplayString(1,show_a);
		//LCD_Cursor(1);
	//	LCD_DisplayString(1,show_a);
		//	LCD_DisplayString(1," ");
		//	LCD_Cursor(13);
		//	LCD_DisplayString(1,"cm");
		//	LCD_Cursor(1);
		state=sm1_display;

		break;
		
		
		
		
		
		
		default:
		state=sm1_display;
		
		break;
		
		
	}
	
	return state;
};




int main(void)
{
	
	
	DDRA=0xFF; PORTA=0X00;
	DDRC=0xFF; PORTC=0x00;

		DDRD = 0xFB; 
		
		LCD_init();
	delay_ms(50);

	LCD_ClearScreen();
	
	TimerSet(timerPeriod);
	TimerOn();
	
	
	
	
	
	
	


	EIMSK |= _BV(INT0);  ////INT0 interrupt enable. 
	//GICR |= 1<<INT0;
//	MCUCR |= 1<<ISC00;

	EICRA |= _BV(ISC00);
	sei();
	
	
	while(1)
	{
			PORTD |= 1<<PIND0;
			_delay_us(15);
			
			PORTD &= ~(1<<PIND0);
			count_a = pulse/58;
			
			
		
	}
	
	
	while(1)
	{
	//	LCD_ClearScreen();
		
		
		unsigned char i = 0;
		tasks[i].state = sm1_display;
		tasks[i].period = 10;
		tasks[i].elapsedTime = 0;
		tasks[i].TickFct = &SMTick1; 
		
		
	//	delay_ms(100);
		//	LCD_DisplayString(1,"Distance=");
	//	itoa(count_a,show_a,10);
	//	LCD_DisplayString(1,show_a);
	
				
	
	}
}

ISR(INT0_vect)
{
	if(i == 1)
	{
		TCCR1B = 0;
		pulse = TCNT1;
		TCNT1 = 0;
		i = 0;
	}

	if(i==0)
	{
		TCCR1B |= 1<<CS10;
		i = 1;
	}
}