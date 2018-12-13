#include <avr/io.h>
#include "io.c"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

  #define tasksNum 2
  #define timerPeriod 1
 #include "scheduler.h"
#define F_CPU 8000000UL
static volatile  unsigned long pulse = 0;
static volatile int i = 0;


unsigned char me =1;
unsigned long count_a = 0;
unsigned long count_b = 0;
unsigned long count_c = 0;
static volatile int TimerOverflow = 0;
char show_a [32];
unsigned char t=0;

	//unsigned char mdata [4]= { 0x05, 0x09,0x0A, 0x06};
//unsigned char mdata [8]= {0x01, 0x05, 0x04, 0x06, 0x02, 0x0A, 0x08, 0x09};
//unsigned char mdata [8]={0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09};
unsigned char r=0;

unsigned char triggered=0;




void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << COM0A0 |  1 << WGM01 | 1 << WGM00);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS02) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	ICR1=  160;
	OCR0A = 11;
	
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}




enum SM1state {sm1_display, sm2_display} one;  //display state machine
int SMTick1(int state) {
	
	
	
	// === Local Variables ===

	
	// === Actions ===

	switch (one) {
		//unsigned char i=0;
		case sm1_display:   //populates character
		if (triggered==1){
			
			PORTB=0x01;
			
			
			if (count_c <20){
				OCR0A = 15;  //16 is up 12 center 8 is down
			}
			
			if (count_c <40 &&count_c >20 ){
				OCR0A = 14;  //16 is up 12 center 8 is down
			}
			
			if (count_c <80 &&count_c >40 ){
				OCR0A = 13;  //16 is up 12 center 8 is down
			}
			
			triggered=0;
			
			
			
			
			
		}
		else{
			PORTB=0x00;
		//	OCR0A = 12;
			
			
		}

		state=sm1_display;

		break;
		case sm2_display:
		
		break;
		
		
		
		default:
		state=sm1_display;
		
		break;
		
		
	}
	
	return state;
};
enum uart_state{init, next} state;

int uart_tick(int state)
{
	
	static volatile unsigned char m=0;
	static volatile unsigned char mdata [5]= { 0x06, 0x0A,0x09, 0x05, 0x00};
//	static volatile unsigned char mdata [5]= { 0x05, 0x09,0x0A, 0x06, 0x00};
	switch(state)
	{
		case init:
		//PORTC= mdata[0];
		state=next;
		break;
		case next:
		if (triggered==1 || r==1){
			r=1;
		if (t<70){
			if (m==0){
				
			PORTC=mdata[m];
			m=4;
			state=next;
			t++;
			
		}
		else{
		//	delay_ms(1);
			PORTC=mdata[m];
			m--;
			state=next;
		}
		}
		
		if (t>=70 && t<140){
			
			if (m>=7){
				m=0;
				PORTC=mdata[m];
				state=next;
				t++;
				
			}
			else{
				PORTC=mdata[m];
				m++;
				state=next;
			}
			
			
		}
		
		if (t>=140){
			t=0;
			r=0;
		}
		
		
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
	
	
	DDRA=0xFF; PORTA=0X00;
	DDRB=0xFF; PORTB=0X00;
	DDRC=0xFF; PORTC=0x00;

	DDRD =   0b11111011; // 0xFB;
	
 	LCD_init();
 _delay_ms(50);
// 

	

	
	 EIMSK |= (1<<INT0);  ////INT0 interrupt enable.
	//GICR |= 1<<INT0;
	//	MCUCR |= 1<<ISC00;

 	 EICRA |= (1 <<ISC00);
//	  	EICRA &= ~(1 <<ISC11);
	sei();
	
	
	
	TimerSet(timerPeriod);
	TimerOn();

	
	
	
	PWM_on();
	
	unsigned char i = 0;
	tasks[i].state = sm1_display;
	tasks[i].period = 1000;		//100mS period!
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &SMTick1;
	++i;
	
	
	tasks[i].state = init;
	tasks[i].period = 3;		//100mS period!
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &uart_tick;
	i=0;


	while(1)
	{

		count_b = count_a;
		
		PORTD |= 1<<PIND0;
		_delay_us(15);
		
		PORTD &= ~(1<<PIND0);
		delay_ms(120);
		
		count_a = pulse/580;
		me++;
		
		
		
		
			delay_ms(120);
		

		ltoa(count_a,show_a,10);
		LCD_DisplayString(1,show_a);



		if (count_b> count_a+10){														//change to a+10
			
			triggered=1;
			count_c = count_a;
		}
		else{
			
		}

	
		
		
		
	}
}

ISR(INT0_vect)
{
	if(i == 1)
	{
		TCCR3B = 0;
		pulse = TCNT3 + (65535 * TimerOverflow);	;
		TCNT3 = 0;
		
		TimerOverflow = 0;
		i = 0;
	}

	if(i==0)
	{
		TCCR3B |= 1<<CS30;
		TIMSK3 = (1 << TOIE1);
		if (TCNT3 ==65534){
				TimerOverflow++;
				TCNT3=0;
				TCCR3B=0;
		}
		i = 1;
	}
} 

ISR(TIMER3_OVF_vect)
{
	TimerOverflow++;	/* Increment Timer Overflow count */
}
