#include "servo.h"
#include "uart.h"

#define MAX	500
#define MIN	250
#define MID	375

void SERVO_set_position(uint8_t pos) // 0-255
{
	double p = ((double)pos)/ 255;
	uint16_t pulse_width = MIN + (uint16_t)(MAX-MIN)*p;
	
	// clamp to min/max
	pulse_width = pulse_width > MAX ? MAX : pulse_width;
	pulse_width = pulse_width < MIN ? MIN : pulse_width;

	OCR1A = pulse_width;
}


void SERVO_init()
{
	// clear OC1A on compare match, set OC1A at BOTTOM (non-inverting)
	TCCR1A |= (1 << COM1A1) | (0 << COM1A0);
	
	// set to mode 14 (Fast PWM)
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
	TCCR1A |= (1 << WGM11) | (0 << WGM10);
	
	// set prescaler to 1/64
	TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);
	
	// set TOP = 5000, frequency = 50 Hz
	ICR1 = F_CPU/(64*50);
	
	// set OC1A (PB5, or number 11 on arduino shield) to output
	DDRB |= (1 << DDB5);
	
	// set middle position of servo
	OCR1A = MID;
	
	printf("[SERVO] INFO: initialization done\r\n");
}
