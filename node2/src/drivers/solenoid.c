#include "solenoid.h"
//#include "../setup.h"

#include <util/delay.h>
#include <avr/interrupt.h>

int solenoid_timer_flag = 0;

void SOLENOID_init( void )
{
	// Set A1: PF1 as output
	DDRF |= (1 << DDF1);
	PORTF |= (1 << PF1);
	
	SOLENOID_timer_init();
}

void SOLENOID_pulse(uint8_t shoot)
{
	// This function should only be called if shoot has changed from 0 to 1
	// (message received from 162 only when this happens)
	if (shoot){
		//set pin to 0
		PORTF &= ~(1 << PF1);
	}
}

void SOLENOID_timer_init( void )
{
	//CTC mode
	TCCR4B |= (1 << WGM42);
	
	// Prescaler = 256
	TCCR4B |= (1 << CS42);
	
	// Set output compare value = 20834, corresponds to f = 3Hz
	OCR4A = 0x5162;
	// Start timer
	TIMSK4 |= (1 << OCIE4A);
}

ISR(TIMER4_COMPA_vect)
{
	PORTF |= (1 << PF1);
}