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
	CAN_send_msg(&mode_msg);
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
		CAN_send_msg(&send_msg);
			
		// read IR messages
		if (CAN_get_latest_msg(&receive_msg))
		{
			score++;
		}
		
		_delay_ms(LOOP_TIME);
	}
	
	return (game_score_t){.failures=score, .time_ms=time_ms, .valid=1};
}

game_score_t GAME_pos_control()
{
	return game(true);
}

game_score_t GAME_no_ctrl()
{
	return game(false);
}
