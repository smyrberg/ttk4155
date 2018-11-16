#include "solenoid.h"
#include "common/uart.h"

#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>

void SOLENOID_init()
{
	// set A1: PF1 as output
	DDRF |= (1 << DDF1);
	PORTF |= (1 << PF1);
	printf("[SOLENOID] INFO: initialization done\r\n");
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
