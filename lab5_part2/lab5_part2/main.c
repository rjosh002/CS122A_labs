/*
 * rjosh002_lab5_part1.c
 *
 * Created: 10/23/2018 4:09:27 PM
 * Author : Rohit
 */ 

#include <avr/io.h>

#define tasksNum 1
#define timerPeriod 1


#include <avr/io.h>
#include <avr/interrupt.h>


#include "scheduler.h"


#define buttonA ~PINA & 0x01
#define buttonB ~PINA & 0x02
#define buttonC ~PINA & 0x04
#define buttonD ~PINA & 0x08
#define buttonE ~PINA & 0x10
#define buttonF ~PINA & 0x20


unsigned char i=0;
unsigned int t=0;
unsigned char mdata [8]= {0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09};

enum uart_state{init, check, next, reverse, wait} state;

int stepper_tick(int state)
{
	switch(state)
	{
		case init:
		PORTB= 0;
		state=check;
		break;
		
		case check:
		PORTB=0;
		if (buttonA || buttonB || buttonC){
			if (t==0){
				if(buttonA){t= (90 / 5.625) * 64;}
				if(buttonB){t= (180 / 5.625) * 64;}
				if(buttonC){t= (360 / 5.625) * 64;}
			
			}
			state=next;
			
				}
		
		if (buttonD || buttonE || buttonF){
			if (t==0){
				if(buttonD){ t= (90 / 5.625) * 64;}
				if(buttonE){t= (180 / 5.625) * 64;}
				if(buttonF){t= (360 / 5.625) * 64;}
			}
			state=reverse;
			
				}
			
		
		break;
		
		
		case next:
			if (i>=7){
				i=0;
		//	PORTB=mdata[i];
			state=next;
		}
		else if(t>0){
			PORTB=mdata[i];
			i++;
			t--;
			state=next;
		}
		
		
		else  {
			state = wait;
		}
		break;
		
		
		
		
		case reverse:
			
			if (i==0){
				i=8;
				//PORTB=mdata[i];
				state=reverse;
			}
			else if(t>0){
				PORTB=mdata[i];
				i--;
				t--;
				state=reverse;
			}
			
		
		else  {
			state = wait;
		}
		
		break;
		
		
		
		case wait:
		if (~buttonA && ~buttonB && ~buttonC && ~buttonD && ~buttonE && ~buttonF){
			t=0;
			state=check;
			
		}
		else{
			state=wait;
		}
		break;

		default:
		state=init;
		break;
	}
	

	return state;
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	DDRA = 0x00; PORTA = 0xFF;
	
	
	TimerSet(timerPeriod);
	TimerOn();
	
	unsigned char i = 0;
	tasks[i].state = init;
	tasks[i].period = 1;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &stepper_tick;
	
	
	while(1)
	{
	}
}

