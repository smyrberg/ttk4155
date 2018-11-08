
#ifndef SOLENOID_H_
#define SOLENOID_H_

#include <avr/io.h>

void SOLENOID_init( void );
void SOLENOID_shoot();
void SOLENOID_timer_init( void );

#endif /* SOLENOID_H_ */