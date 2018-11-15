#define F_CPU 16000000
#include "drivers/servo.h"
#include "drivers/pwm.h"
#include "drivers/ir.h"
#include "drivers/motor.h"
#include "drivers/solenoid.h"

#include "common/uart.h"
#include "common/spi.h"
#include "common/can.h"

#include <avr/interrupt.h>
#include <util/delay.h>

// local declarations
void nop_handler(can_message_t *msg){};
void on_incoming_CAN(can_message_t * msg);
static void broadcast_IR();
typedef enum {no_control=0, pid_position_control=1} controller_mode;
//typedef enum  {node2_on, node2_off} node_mode;

can_message_t *g_multi_msg;
can_message_t *g_other_msg;

can_message_t get_latest_cmd(){
	cli();
	printf("wtf\r\n");
	can_message_t latest;
	latest.id = g_multi_msg->id;
	latest.length = g_multi_msg->length;
	for (int i = 0; i < 8; i++)
	{
		latest.data[i] = g_multi_msg->data[0];
	}
	sei();
	return latest;
}


int main()
{
	// initialize
	cli();
	UART_Init(UBRR);
	SPI_init();
	CAN_init(0);
	CAN_set_receive_handler(nop_handler); //ignore can during motor init
	PWM_init();
	IR_init();
	MOTOR_init();
	SOLENOID_init();
	sei();
	
	g_multi_msg = &(can_message_t){.id=CAN_MSG_MULTI_CMD, .length=4, .data={0,0,127,0}};
	g_other_msg = NULL;	
	
	MOTOR_find_limits();
	//MOTOR_start_controller();
	
	printf("init done Node2\r\n");
	
	CAN_set_receive_handler(&on_incoming_CAN);
	
	int fails = 0;
	can_message_t multi_msg, other_msg;
	while(1)
	{	
		if (g_other_msg)
		{	
			other_msg = *g_other_msg;
			if (other_msg.id == CAN_MSG_START_NODE2)
			{
				controller_mode mode= other_msg.data[0];
				printf("mode: %d\r\n", mode);
				g_other_msg = NULL;
				run_game(mode);
			}
		}
		_delay_ms(25);
	}
}
		
		
void run_game(controller_mode mode)
{
	// init controller
	if (mode == pid_position_control)
	{
		printf("PID mode on\r\n");
		MOTOR_start_controller();
	}
	else
	{
		printf("no controller on\r\n");
	}
	can_message_t multi_msg;
	while(1)
	{
		multi_msg = get_latest_cmd();
		CAN_default_receive_handler(&multi_msg);
		
		switch(mode)
		{
			case no_control:
				motor_cmd_no_control(multi_msg);
				break;
			case pid_position_control:
				motor_cmd_pid(multi_msg);
				break;
		}
		
		SERVO_set_pos(multi_msg.data[2]);
		if(multi_msg.data[3]) { SOLENOID_shoot(); }

		// TODO: send IR
	
		if (g_other_msg)
		{
			can_message_t other_msg = *g_other_msg;
			if (other_msg.id == CAN_MSG_STOP_NODE2)
			{
				MOTOR_stop();
				return;
			}
		}
	}
}

void motor_cmd_no_control(can_message_t msg)
{
	MOTOR_set_dir(msg.data[0]);
	MOTOR_set_vel(msg.data[1]);
}

void motor_cmd_pid(can_message_t msg)
{
	// 0/1  0-128
	uint8_t pos = !msg.data[0] ? 128+msg.data[1] : 128-msg.data[1];
	MOTOR_set_position(pos);
}


void on_incoming_CAN(can_message_t * msg)
{
	if (msg->id == CAN_MSG_MULTI_CMD)
	{
		g_multi_msg = msg;
	}
	else {
		g_other_msg = msg;
	}
}