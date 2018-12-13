/*
 * Rjosh002_lab4_part1.c
 *
 * Created: 10/22/2018 9:50:27 PM
 * Author : Rohit
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
enum State {off,on, three} state;


void LEDS_Tick()
{
	//Actions
	switch(state)
	{
		case off:
		PORTB = 0x01;
			state = on;
		break;
		case on:
		if (PORTB==0x80){
			PORTB =0x40;
			state=three;
		}
			else{
		PORTB=(PORTB<<1);
			state = on;
			}
		break;
		
		
		case three:
		
		if(PORTB==0x01){
			
			PORTB=0x02;
			state=on;
		}
		else {
			PORTB=(PORTB >>1);
			state=three;
		}
		break;
		default:
		state=off;
		break;
	}
		return state;
}
void LedWait()
{
	for(;;)
	{
		LEDS_Tick();
		vTaskDelay(500);
	}
}
void Waitpulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedWait, (signed portCHAR *)"LedWait", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}
int main(void)
{
	DDRA = 0x00; PORTA=0xFF;
	DDRB = 0xFF; PORTB=0x00;
	Waitpulse(1);
	vTaskStartScheduler();
	
	return 0;
}