
#ifndef MOTOR_H_
#define MOTOR_H_

#include <avr/io.h>

typedef enum motor_mode_t {
	MOTOR_mode_pid = 0,
	MOTOR_mode_no_ctrl
} motor_mode_t;

typedef enum motor_direction_t {
	MOTOR_right = 0,
	MOTOR_left
} motor_direction_t;

void MOTOR_init();
void MOTOR_find_limits();
void MOTOR_set_mode(motor_mode_t mode);
motor_mode_t MOTOR_get_mode();
void MOTOR_set_position(uint8_t position);
void MOTOR_set_speed(uint8_t speed);
void MOTOR_set_direction(motor_direction_t direction);

#endif /* MOTOR_H_ */