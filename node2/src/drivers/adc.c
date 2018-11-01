#include "adc.h"

void ADC_init()
{
	// Enable ADC and set prescaler to 128
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t ADC_read()
{
	// Reference selection: AVCC w/ external capacitor at AREF. Left adjust result
	ADMUX |= (1 << REFS0) | (1 << ADLAR);
	
	// Start conversion
	ADCSRA |= (1 << ADSC);
	
	// Wait until conversion is complete
	while(ADCSRA & (1 << ADSC));
	
	// read 12 bit into 16 bit return value
	return ADC>>4;
}