#include <avr/io.h>

#ifndef PWM_H_
#define PWM_H_


void PWM_init( void );
void PWM_set_pulse_width(uint16_t pulse_width);


#endif /* PWM_H_ */