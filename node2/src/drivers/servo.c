#include "servo.h"
#include "pwm.h"

#include <stdlib.h>

void SERVO_set_pos(uint8_t pos) // 0-255
{
	double p = ((double)pos)/ 255;
	uint16_t pulse_width = MIN + (uint16_t)(MAX-MIN)*p;
	
	// clamp to min/max
	pulse_width = pulse_width > MAX ? MAX : pulse_width;
	pulse_width = pulse_width < MIN ? MIN : pulse_width;

	PWM_set_pulse_width(pulse_width);

}

void SERVO_set_position(uint8_t pos)
{
	uint16_t pulse_width = MIN + (uint16_t)(255-pos);
	
	// clamp to min/max
	if ( pulse_width > MAX){ 
		pulse_width = MAX; 
	}
	else if ( pulse_width < MIN ){ 
		pulse_width = MIN; 
	}
	
	PWM_set_pulse_width(pulse_width);
}