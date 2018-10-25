#ifndef SERVO_H_
#define SERVO_H_


#define MAX		0x01F4	// 500
#define MIN		0x00FA	// 250

#define POSITIVE 1
#define NEGATIVE 0

#include <avr/io.h>

//void SERVO_set_position(uint8_t pos);
void SERVO_set_pos(uint8_t pos);

#endif /* SERVO_H_ */