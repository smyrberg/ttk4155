#include "spi.h"
#include "bitmanip.h"
#include <avr/io.h>
#include <util/delay.h>

/* setup.h stuff
#define F_CPU 4915200
#define FOSC 4915200	// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
*/

void SPI_init()
{
	// Enable SPI, Master, set clock rate
	SPCR = (1 << SPE)|(1 << MSTR)|(1 << SPR0);
	
	// Set MOSI, SCK and SS output
	DDRB = (1 << DDB5)|(1 << DDB7)|(1 << DDB4);
}


uint8_t SPI_transmit_receive(uint8_t data)
{
	// Start transmission (write to data register)
	SPDR = data;
	// Wait for transmission complete
	while(!(SPSR & (1 << SPIF)));
	
	return SPDR;
}

// Set _SS to 1 or 0
void SPI_set_ss(int val)
{
	if (val == 1){
		set_bit(PORTB, PB4);
	}
	else if (val == 0){
		clear_bit(PORTB, PB4);
	}
}