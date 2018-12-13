/*
 * rjosh002_lab5_part1.c
 *
 * Created: 10/23/2018 4:09:27 PM
 * Author : Rohit
 */ 

#include <avr/io.h>

#define tasksNum 2
#define timerPeriod 1

#define columnsize 3
#define rowsize 3
#include <avr/io.h>
#include <avr/interrupt.h>


#include "scheduler.h"


#define C0 (~PINB & 0x01)
#define C1 (~PINB & 0x02)
#define C2 (~PINB & 0x04)
#define C3 (~PINB & 0x08)


unsigned char columns[columnsize] = {0xEF, 0xF7, 0xFB} ;
unsigned char rows[rowsize] = {0x7C, 0x44, 0x7C};
	
enum SM1_States {sm1_display} state;  //display state machine
int SMTick1(int state) {
	
	
	
	// === Local Variables ===

	static unsigned char i=0;
	static unsigned char which=0;
	static unsigned char wich=0;
	static unsigned char charit=0 ;
	static unsigned char blip ;
	static unsigned char rich ;
	static unsigned char mip;
	static unsigned char stip;
	static unsigned char grip;
	
	
	// === Actions ===

		switch (state) {
			//unsigned char i=0;
			case sm1_display:   //populates character
			PORTA=(0x00);
			PORTD=(0xFF);
			
			
			
			
			
			
			
			

			if(i<rowsize){
				

				//PORTD=0xFF;
				PORTA =(rows[i]); // PORTA displays column pattern
				PORTD = columns[i];  // PORTB selects column to display patterns
				//	 PORTD=0xFE;
				// PORTA = 0x01; // PORTA displays column pattern
				// PORTD=0x7F;
				
				//	  transmit_data(column_sel);
				i++;
				state=sm1_display;
				
			}
			
			else{
				i=0;
			//	state=sm3_display;

			}

			break;
			
			
			
			
			
			
			default:
			state=sm1_display;
			
			break;
			
			
		}
	
	return state;
};


enum button_States {wait,left,right,up, down} bstate;		//charachter movement machine
int Button(int state) {
	unsigned char i=0;
	unsigned char r=0;
	switch (bstate) {
		
		
		case wait:
		
		if (C0) {
			bstate=left; 
		}
		else if(C1){
			bstate=right;
		}
	else if(C2){
		bstate=up;
	}
	else if(C3){
		bstate=down;
	}
		else{
			bstate=wait;
		}
		break;
		
		
		case left:
		if(C0 && columns[0] != 0x7F ){

			for (i=0; i<columnsize;i++){
				
				columns[i] = (columns[i]<< 1) | 0x01;
				
			}
		}
		i=0;
		bstate=wait;
		break;
		
		
		
		case right:
		if(C1 && columns[columnsize-1] != 0xFE )
		{
			for (i=0; i<columnsize;i++)
			{
				columns[i] = (columns[i]>> 1) | 0x80;
			}
		}
		i=0;
		bstate=wait;
		break;
		
		
			case up:
			if(C2 && rows[rowsize-1] != 0x1F )
			{
				for (i=0; i<rowsize;i++)
				{
					rows[i] = (rows[i]>> 1);
				}
			}
			i=0;
			bstate=wait;
			break;
			case down:
			if(C3 && rows[rowsize-1] != 0xF8 )
			{
				for (i=0; i<rowsize;i++)
				{
					rows[i] = (rows[i]<< 1);
				}
			}
			i=0;
			bstate=wait;
			break;
		
		default:
		bstate=wait;
		
		break;
		
	}
}
int main(void)
{
	DDRB = 0x00; PORTB = 0xFF; // PORTB set to output, outputs init 0s
	DDRA = 0xFF; PORTA = 0x00;
	
			DDRD = 0xFF; PORTD = 0x00;
	
	
	TimerSet(timerPeriod);
	TimerOn();
	
	unsigned char i = 0;
	tasks[i].state = sm1_display;
	tasks[i].period = 1;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &SMTick1;
	i++;
		tasks[i].state = wait;
		tasks[i].period = 50;
		tasks[i].elapsedTime = 0;
		tasks[i].TickFct = &Button;
		
	
	
	while(1)
	{
	}
}

