
#ifndef MOTOR_H_
#define MOTOR_H_

#include <avr/io.h>

typedef enum {MOTOR_right, MOTOR_left} MOTOR_Direction;

void MOTOR_init( void );
void MOTOR_find_limits( void );
void MOTOR_set_dir(MOTOR_Direction dir);
void MOTOR_set_max_velocity(int speed);
void MOTOR_set_vel(uint8_t vel);
int16_t MOTOR_read_encoder( void );
int MOTOR_read_scaled_encoder( void );

void MOTOR_start_controller();
void MOTOR_stop();
void MOTOR_set_position(uint8_t reference_pos);


#endif /* MOTOR_H_ */