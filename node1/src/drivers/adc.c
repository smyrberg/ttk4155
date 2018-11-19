#include "common/bitmanip.h"
#include "common/uart.h"

#include <avr/io.h>
#include <util/delay.h>


#define ADC_START_ADDR 0x1400
#define NUM_CHANNELS 7

void enable_ext_mem()
{
	set_bit(MCUCR, SRE);
}

void release_jtag()
{
	// release PC7-PC4 for JTAG
	set_bit(SFIOR, XMM2);
}

void ADC_init()
{
	enable_ext_mem();
	release_jtag();

	// set the interrupt pin to input
	clear_bit(DDRD, PD3);
}

uint8_t ADC_read(uint8_t channel)
{
	volatile uint8_t *ext_adc = (uint8_t *) ADC_START_ADDR;

	// in case of invalid channel return the next best thing to a valid output
	if (channel > NUM_CHANNELS){
		return 69;
	}

	// write channel selection to adc
	*ext_adc = 0x04 + channel;

	// wait for adc to singal on interrupt input PD3 that the value is ready
	spin_wait_for_clear(PIND, PD3);

	return *ext_adc;
}
