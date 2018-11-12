#include "game.h"

#include "drivers/oled.h"
#include "drivers/joystick.h"

#include "common/uart.h"
#include "common/can.h"

#include <avr/delay.h>

#define GAME_ITERATIONS 10000


game_score_t g_score = {.time=0, .failures=0, .valid=1};
uint8_t g_game_ready = 0;

static void print_instructions()
{
	OLED_pos(1, ARROW_WIDTH);
	OLED_printf("left/right to drive");
	
	OLED_pos(2, ARROW_WIDTH);
	OLED_printf("up/down to aim");
	
	OLED_pos(3, ARROW_WIDTH);
	OLED_printf("right button to shoot");
}

static can_message_t pos2motorcmd(JOY_position_t pos)
{
	uint8_t xpos = pos.x;
	uint8_t direction = xpos < 127;
	
	uint8_t speed = 2 * abs(127-xpos);
	speed = (speed < 15) ? 0 : speed;
	
	can_message_t msg = {.id=CAN_MSG_MOTOR_CMD, .length=2};
	msg.data[0] = direction;
	msg.data[1] = speed;

	return msg;
}

static can_message_t pos2servocmd(JOY_position_t pos)
{
	return (can_message_t){.id=CAN_MSG_SERVO_CMD, .length=1, .data=pos.y};
}

static can_message_t create_solenoid_msg()
{
	return JOY_right_button() ? 
		(can_message_t){.id=CAN_MSG_SOLENOID_CMD, .length=0}: 
		(can_message_t){.id=CAN_MSG_NOP_CMD};
}

void can_receive_handler(can_message_t *msg)
{
	switch(msg->id)
	{
		case CAN_MSG_IR_DETECTION:
			g_score.failures++;
			break;
		case CAN_MSG_MOTOR_INIT_DONE:
			g_game_ready = 1;
			break;
	}
	
}


game_score_t GAME_play()
{
	g_score = (game_score_t){.failures = 0, .time=0, .valid=1};
	// register receive handler
	CAN_set_receive_handler(can_receive_handler);
	
	OLED_reset();
	OLED_pos(0, ARROW_WIDTH);
	OLED_printf("=== LETS PLAY ===");
	print_instructions();
	
	JOY_position_t joy_pos;
	can_message_t motor_msg, servo_msg, solenoid_msg;
	float done = 0;
	for(int i = 0; i < GAME_ITERATIONS;i++)
	{
		// read from IO board
		joy_pos	= JOY_get_position();
		motor_msg	 = pos2motorcmd(joy_pos);
		servo_msg	 = pos2servocmd(joy_pos);
		solenoid_msg = create_solenoid_msg();
		 
		// send CAN messages
		CAN_message_send(&motor_msg);
		CAN_message_send(&servo_msg);
		if (solenoid_msg.id == CAN_MSG_SOLENOID_CMD) { CAN_message_send(&solenoid_msg); }
		
		// update screen
		done = (i / 10000.0 );
		OLED_pos(4,ARROW_WIDTH);
		OLED_printf("fail: %02d left: %02d/100 " , g_score.failures, (int)(done*100.0));

	
	}
	return g_score;
}