#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#define X_POS_CHANNEL 0
#define Y_POS_CHANNEL 1
#define SLIDER_LEFT_CHANNEL 2
#define SLIDER_RIGHT_CHANNEL 3

#define JOY_BUTTON 0
#define LEFT_BUTTON 1
#define RIGHT_BUTTON 2

typedef enum {NEUTRAL, LEFT, RIGHT, UP, DOWN} JOY_direction_t;


typedef struct JOY_pos {
	int y;
	int x;
	} JOY_position_t;
	
void JOY_init(void);
int JOY_button(int button);
JOY_position_t JOY_read_adc(void);
JOY_position_t JOY_get_position();
JOY_direction_t JOY_get_direction();


#endif /* JOYSTICK_H_ */