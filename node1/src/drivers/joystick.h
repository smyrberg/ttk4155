#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#define X_POS_CHANNEL 1
#define Y_POS_CHANNEL 0
#define SLIDER_LEFT_CHANNEL 2
#define SLIDER_RIGHT_CHANNEL 3

#define JOY_BUTTON 0
#define LEFT_BUTTON 1
#define RIGHT_BUTTON 2

typedef enum {NEUTRAL, LEFT, RIGHT, UP, DOWN} JOY_direction_t;
	
#include <avr/io.h>


typedef struct JOY_pos {
	uint8_t x;
	uint8_t y;
} JOY_position_t;

void JOY_init(void);
int JOY_right_button();

JOY_position_t JOY_get_position();
JOY_direction_t JOY_get_direction();


#endif /* JOYSTICK_H_ */