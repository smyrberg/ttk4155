#include "scores.h"
#include <avr/interrupt.h>

static void EEPROM_write(unsigned int addr, unsigned char data)
{
	// Wait for completion of previous write
	while(EECR & (1<<EEWE));
	
	// Set up address and data registers
	EEAR = addr;
	EEDR = data;
	
	// Write logical one to EEMWE
	EECR |= (1<<EEMWE);
	
	// Start eeprom write by setting EEWE
	EECR |= (1<<EEWE);
}

unsigned char EEPROM_read(unsigned int addr)
{
	// Wait for completion of previous write
	while(EECR & (1<<EEWE));
	
	// Set up address register
	EEAR = addr;
	
	// Start eeprom read by writing EERE
	EECR |= (1<<EERE);
	
	// Return data from data register
	return EEDR;
}

void SCORE_delete()
{
	for (int i=0; i < sizeof(SCORE_info_t) * NUM_SCORES; i++ )
	{
		EEPROM_write(i, 0);
	}
}