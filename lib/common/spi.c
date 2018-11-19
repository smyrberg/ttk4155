#include "uart.h"
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
	// Set MOSI, SCK and SS output
	#if defined(__AVR_ATmega162__)
		printf("init atmega162 spi\r\n");
		DDRB |= (1 << DDB5)|(1 << DDB7)|(1 << DDB4);
	#endif
	#if defined(__AVR_ATmega2560__)
		printf("init atmega2560 spi\r\n");
		DDRB |= (1 << DDB2)|(1 << DDB1)|(1 << DDB0) | (1 << DDB7);
	#endif
	
	// Enable SPI, Master, set clock rate
	SPCR = (1 << MSTR)|(1 << SPR0);
	SPCR |= (1 << SPE);
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
	#if defined(__AVR_ATmega162__)
	if (val == 1){
		set_bit(PORTB, PB4);
	}
	else if (val == 0){
		clear_bit(PORTB, PB4);
	}
	#endif

	#if defined(__AVR_ATmega2560__)
	if (val == 1){
		set_bit(PORTB, PB7);
	}
	else if (val == 0){
		clear_bit(PORTB, PB7);
	}
	#endif
}