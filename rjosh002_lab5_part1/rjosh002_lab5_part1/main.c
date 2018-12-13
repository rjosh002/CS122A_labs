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

unsigned char t=0;
unsigned char i=0;
unsigned char mdata [4]= { 0x01, 0x02, 0x04, 0x08};
//unsigned char mdata [8]= {0x01, 0x05, 0x04, 0x06, 0x02, 0x0A, 0x08, 0x09};
//	unsigned char mdata [4]= { 0x05, 0x09,0x0A, 0x06};
//	unsigned char mdata [8]={0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09};
unsigned char r=0;
enum uart_state{init, next, toggle} state;

int uart_tick(int state)
{
	switch(state)
	{
		case init:
		PORTC= mdata[0];
		state=next;
		break;
		case next:
	//	if (t<70){
			if (i==0){
				i=8;
			PORTC=mdata[i];
			state=next;
			t++;
			
		}
		else{
			PORTC=mdata[i];
			i--;
			state=next;
		}
	//	}
		
	/*	if (t>=70 && t<140){
			
			if (i>=7){
				i=0;
				PORTC=mdata[i];
				state=next;
				t++;
				
			}
			else{
				PORTC=mdata[i];
				i++;
				state=next;
			}
			
			
		}*/
	//else{}
	
		break;
		
	/*	case toggle:
		if (buttonB && ~buttonA){
			if (i==0){
				i=8;
				PORTB=mdata[i];
				state=toggle;
			}
			else{
				PORTB=mdata[i];
				i--;
				state=toggle;
			}
			
		}
		else  {
			state = next;
		}
		
		break; */
		
		
		
		default:
		state=init;
		break;
	}
	

	return state;
}

int main(void)
{
	DDRC = 0xFF; PORTC = 0x00; // PORTB set to output, outputs init 0s
	DDRA = 0x00; PORTA = 0xFF;
	
	
	TimerSet(timerPeriod);
	TimerOn();
	
	unsigned char i = 0;
	tasks[i].state = init;
	tasks[i].period = 500;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &uart_tick;
	
	
	while(1)
	{
	}
}

