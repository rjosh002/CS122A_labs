/*
 * Rjosh002_lab4_part2.c
 *
 * Created: 10/22/2018 9:50:27 PM
 * Author : Rohit
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
enum oneeState {offone,onone} onestate;
enum twooState {offtwo,ontwo} twostate;
	enum sState {off,on} state;

void ONE_Tick()
{
	//Actions
	switch(onestate)
	{
		case offone:
		PORTB = PORTB & 0xFE;
			onestate = onone;
		break;
		case onone:
		PORTB |= 0x01;
			onestate = offone;
		break;
		default:
		onestate=offone;
		break;
	}
	//	return onestate;
}
void TWO_Tick()
{
	//Actions
	switch(twostate)
	{
		case offtwo:
		PORTB =PORTB & 0xFB;
		twostate = ontwo;
		break;
		case ontwo:
		PORTB |= 0x04;
		twostate = offtwo;
		break;
		default:
		twostate=offtwo;
		break;
	}
	//return twostate;
}
void THREE_Tick()
{
	//Actions
	switch(state)
	{
		case off:
		PORTB = PORTB & 0xEF;
		state = on;
		break;
		case on:
		PORTB |= 0x10;
		state = off;
		break;
		default:
		state=off;
		break;
	}
	//return state;
}
void oneWait()
{
	onestate = offone;
	for(;;)
	{
		ONE_Tick();
		vTaskDelay(500);
	}
}
void twoWait()
{
	twostate = offtwo;
	for(;;)
	{
		TWO_Tick();
		vTaskDelay(1000);
	}
}
void threeWait()
{
	state=off;
	for(;;)
	{
		THREE_Tick();
		vTaskDelay(2500);
	}
}
void Waitpulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(oneWait, (signed portCHAR *)"oneWait", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(twoWait, (signed portCHAR *)"twoWait", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(threeWait, (signed portCHAR *)"threeWait", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}
int main(void)
{
	DDRA = 0x00; PORTA=0xFF;
	DDRB = 0xFF; PORTB=0x00;
	Waitpulse(1);
	vTaskStartScheduler();
	
	return 0;
}