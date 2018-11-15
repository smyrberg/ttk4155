#include "solenoid.h"
#include "common/uart.h"

#define F_CPU 16000000

#include <util/delay.h>
#include <avr/interrupt.h>

int solenoid_timer_flag = 0;
uint8_t g_can_shoot = 1;

void SOLENOID_init( void )
{
	// Set A1: PF1 as output
	DDRF |= (1 << DDF1);
	PORTF |= (1 << PF1);
	
	// SOLENOID_timer_init();
	
}

void SOLENOID_shoot()
{
	cli();
	//set pin to 0
	PORTF &= ~(1 << PF1);
	_delay_ms(100);
	PORTF |= (1 << PF1);
	_delay_ms(20);
	sei();
}

#if 0


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
	printf("hello from intrrupt\r\n");
	g_can_shoot = 1;
}
#endif