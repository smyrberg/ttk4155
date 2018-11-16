#ifndef SERVO_H_
#define SERVO_H_

#include <avr/io.h>

void SERVO_init();
void SERVO_set_position(uint8_t pos);

#endif /* SERVO_H_ */