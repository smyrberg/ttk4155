#include "pwm.h"

#define F_CPU 16000000

void PWM_init( void )
{
	// Clear OC1A on compare match, set OC1A at BOTTOM (non-inverting)
	TCCR1A |= (1 << COM1A1) | (0 << COM1A0);
	
	// Set to mode 14 (Fast PWM)
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
	TCCR1A |= (1 << WGM11) | (0 << WGM10);
	
	// Set prescaler to 1/64
	TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);
	
	// Set TOP = 5000, frequency = 50 Hz
	ICR1 = F_CPU/(64*50);
	
	// Set OC1A (PB5, or number 11 on arduino shield) to output
	DDRB |= (1 << DDB5);
	
	// Set middle position of servo
	PWM_set_pulse_width(0x0177);
}

void PWM_set_pulse_width(uint16_t pulse_width)
{
	OCR1A = pulse_width;
}