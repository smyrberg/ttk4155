#include "game.h"
#include "drivers/oled.h"
#include "drivers/joystick.h"
#include "common/can.h"
#include <avr/delay.h>
#include <stdbool.h>


static can_msg_t make_no_ctrl_cmd_msg(JOY_position_t pos, int btn)
{
	uint8_t xpos = pos.x;
	uint8_t direction = xpos < 127;
		
	uint8_t speed = 2 * abs(127-xpos);
	speed = (speed < 15) ? 0 : speed;
		
	can_msg_t msg = {.id=CAN_msg_command, .length=4};
	msg.data[0] = speed;
	msg.data[1] = direction;
	msg.data[2] = pos.y;
	msg.data[3] = btn;
	return msg;
}

static can_msg_t make_pid_cmd_msg(JOY_position_t pos, int btn)
{
	can_msg_t msg = {.id=CAN_msg_command, .length=4};
	msg.data[0] = 255 - pos.x;
	msg.data[1] = 0;
	msg.data[2] = pos.y;
	msg.data[3] = btn;
	return msg;
}

static void print_score(int score, int time_ms)
{
	OLED_reset();
	OLED_pos(0, 0);
	OLED_printf("GAME");
	
	OLED_pos(2, 0);
	OLED_printf("fails: %d", score);
	
	OLED_pos(3,0);
	OLED_printf("time: %d sec", time_ms/1000);
}

#define GAME_TIME 20000
#define LOOP_TIME 20
game_score_t game(bool pid)
{
	OLED_reset();
	
	// set motor mode
	can_msg_t mode_msg;
	if(pid)
	{
		mode_msg = (can_msg_t){.id=CAN_msg_set_mode, .length=1, .data={0}};
	}
	else
	{
		mode_msg = (can_msg_t){.id=CAN_msg_set_mode, .length=1, .data={1}};
	}
	CAN_message_send(&mode_msg);
	printf("[GAME] INFO: sent motor mode to node2\r\n");
	
	int score = 0;
	
	can_msg_t receive_msg, send_msg;
	volatile int time_ms;
	for(time_ms = 0; time_ms < GAME_TIME; time_ms+=LOOP_TIME)
	{
		print_score(score, time_ms);
		
		// read joystick
		JOY_position_t pos = JOY_get_position();
		int btn = JOY_right_button();
		if(pid)
		{
			send_msg = make_pid_cmd_msg(pos, btn);
		}
		else
		{
			send_msg = make_no_ctrl_cmd_msg(pos, btn);
		}
		CAN_message_send(&send_msg);
			
		printf("reading ir\r\n");
		// read IR messages
		if (CAN_get_latest_msg(&receive_msg))
		{
			score++;
		}
		
		_delay_ms(LOOP_TIME);
	}
	
	return (game_score_t){.failures=score, .time_ms=time_ms, .valid=1};
}

game_score_t GAME_pid()
{
	return game(true);
}

game_score_t GAME_no_ctrl()
{
	return game(false);
}









































#if 0
#include "game.h"

#include "drivers/oled.h"
#include "drivers/joystick.h"

#include "common/uart.h"
#include "common/can.h"

#include <avr/delay.h>
#include <stdlib.h>

#define GAME_ITERATIONS 10000
#define UPDATE_PERIOD_MS 50


game_score_t g_score = {.time_ms=0, .failures=0, .valid=1};
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

static void print_score(game_score_t score)
{
	OLED_pos(4, ARROW_WIDTH);
	OLED_printf("Fails: %2d, time:%2d", score.failures, score.time_ms / 1000);
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

static can_message_t make_multi_msg(JOY_position_t pos, uint8_t shoot)
{
	uint8_t xpos = pos.x;
	uint8_t direction = xpos < 127;
	
	uint8_t speed = 2 * abs(127-xpos);
	speed = (speed < 15) ? 0 : speed;
	
	can_message_t msg = {.id=CAN_MSG_MULTI_CMD, .length=4};
	msg.data[0] = direction;
	msg.data[1] = speed;
	
	msg.data[2] = pos.y;
	msg.data[3] = shoot;

	return msg;
}

static can_message_t pos2servocmd(JOY_position_t pos)
{
	return (can_message_t){.id=CAN_MSG_SERVO_CMD, .length=1, .data=pos.y};
}

static can_message_t create_solenoid_msg()
{
	return JOY_right_button() ? 
		(can_message_t){.id=CAN_MSG_SOLENOID_CMD, .length=1} : 
		(can_message_t){.id=CAN_MSG_NOP_CMD};
}

void can_receive_handler(can_message_t *msg)
{
	switch(msg->id)
	{
		case CAN_MSG_IR_DETECTION:
			g_score.failures++;
			break;
	}
}



static game_score_t run_game(uint8_t control_mode)
{
	// reset score
	g_score = (game_score_t){.failures = 0, .time_ms=0, .valid=1};
	
	// register receive handler
	CAN_set_receive_handler(can_receive_handler);
	
	OLED_reset();
	OLED_pos(0, ARROW_WIDTH);
	OLED_printf("=== LETS PLAY ===");
	print_instructions();
	
	_delay_ms(3000);
	can_message_t start_msg = {.id=CAN_MSG_START_NODE2, .length=1, .data={control_mode}};
	CAN_message_send(&start_msg);
	
	JOY_position_t joy_pos;
	can_message_t multi_msg;
	printf("stating game\r\n");
	while(g_score.time_ms < 20000)
	{
		// read from IO board
		joy_pos	= JOY_get_position();
		multi_msg = make_multi_msg(joy_pos, JOY_right_button());
		CAN_message_send(&multi_msg);
		CAN_default_receive_handler(&multi_msg);

		//update time
		g_score.time_ms += UPDATE_PERIOD_MS;
		
		print_score(g_score);
				
		_delay_ms(UPDATE_PERIOD_MS);
	}
	
	can_message_t stop_msg = {.id=CAN_MSG_STOP_NODE2, .length=0};
	CAN_message_send(&stop_msg);
	return g_score;
}
	
game_score_t GAME_pid()		{ return run_game(1); }
game_score_t GAME_no_ctrl() { return run_game(0); }

/*
game_score_t GAME_no_ctrl()
{
	g_score = (game_score_t){.failures = 0, .time_ms=0, .valid=1};
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
*/

#endif