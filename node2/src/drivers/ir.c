#include "ir.h"
#include "adc.h"
#include <stdlib.h>

#define READINGS 4
uint8_t baseline;


uint16_t IR_read()
{
	int total = 0;
	
	// do multiple readings
	for (int i = 0; i < READINGS; i++)
	{		
		total += ADC_read();	
	}

	return (uint16_t) total/READINGS;
}

void IR_init()
{
	// assumes that line is not broken at initalization
	ADC_init();
	baseline = ADC_read();
}

uint8_t IR_beam_broken()
{
	uint8_t diff = abs(baseline - ADC_read());
	return diff > 2;
}