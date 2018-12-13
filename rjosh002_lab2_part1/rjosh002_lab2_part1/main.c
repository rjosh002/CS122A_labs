#define tasksSize 1
#define timerPeriod 1

#include <avr/io.h>
#include <avr/interrupt.h>

#include "usart_ATmega1284.h"
#include "scheduler.h"


unsigned char tmpB, s_data;

enum uart_state{uart_start, send, toggle};

int uart_tick(int state)
{
	switch(state)
	{
		case uart_start:
		tmpB = 0x00;
		s_data = 0x00;
		state = send;
		break;
		case send:
		if(USART_IsSendReady(1))
		{
			state = send;
			USART_Send(s_data, 1);
			tmpB = s_data;
		}
		state = toggle;
		break;
		case toggle:
		if(USART_HasTransmitted(1))
		{
			if(s_data == 0x00)
			{
				s_data = 0x01;
			}
			else
			{
				s_data = 0x00;
			}
		}
		state = send;
		break;
		default:
		tmpB = 0x00;
		state = uart_start;
		break;
	}
	
	PORTB = tmpB;
	return state;
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	DDRA = 0x00; PORTA = 0xFF;
	DDRD = 0x02; PORTD = 0xFB;
	
	TimerSet(timerPeriod);
	TimerOn();
	
	unsigned char i = 0;
	tasks[i].state = uart_start;
	tasks[i].period = 1000;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &uart_tick;
	
	initUSART(1);
	
	while(1)
	{
	}
}

