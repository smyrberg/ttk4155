#include "ir.h"
#include "common/uart.h"

#include <avr/io.h>

void IR_init()
{
	// Enable ADC and set prescaler to 128
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	printf("[IR] INFO: initialization done\r\n");
}

static uint16_t read_adc()
{
	// Reference selection: AVCC w/ external capacitor at AREF. Left adjust result
	// Using ADC0 (A0 on shield) as input
	ADMUX |= (1 << REFS0) | (1 << ADLAR);
	
	// Start conversion
	ADCSRA |= (1 << ADSC);
	
	// Wait until conversion is complete
	while(ADCSRA & (1 << ADSC));
	
	// read 12 bit into 16 bit return value
	return (uint16_t)(ADC>>4);
}


bool IR_beam_broken()
{
	//printf("%d\r\n",read_adc());
	return read_adc() < 500;
}


