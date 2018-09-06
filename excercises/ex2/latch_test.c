#define F_CPU 4915200
#include <avr/io.h>
#include <util/delay.h>


int latch_main(void)
{
	// port A is output
	DDRA = 0xFF;
	// port E is output (contains pin PE1, which is ALE)
	// ALE = Address Latch Enable
	DDRE = 0xFF;
	PORTE |= (1 <<PE1);
	
	while(1)
	{
		// test the latch by toggling each pin (AD0-AD7) and setting ALE (enable)
		PORTA |= (1 << PA0)|(1 << PA1)|(1 << PA2)|(1 << PA3)|(1 << PA4)|(1 << PA5)|(1 << PA6)|(1 << PA7);

		
		_delay_ms(1000);
		
		PORTA &= ~(1 << PA0);
		PORTA &= ~(1 << PA1);
		PORTA &= ~(1 << PA2);
		PORTA &= ~(1 << PA3);
		PORTA &= ~(1 << PA4);
		PORTA &= ~(1 << PA5);
		PORTA &= ~(1 << PA6);
		PORTA &= ~(1 << PA7);
		
		_delay_ms(1000);
		
		
	}
}