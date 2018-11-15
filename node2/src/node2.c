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
typedef enum control_mode_t {no_control, pid_position_control} control_mode_t;

can_message_t *g_multi_msg;
control_mode_t g_motor_mode;



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
	g_motor_mode = no_control;
	
	//MOTOR_find_limits();
	//MOTOR_start_controller();
	
	
	CAN_set_receive_handler(&on_incoming_CAN);
	
	can_message_t msg;
	while(1)
	{	
		msg = *g_multi_msg;
		
		//CAN_default_receive_handler(&msg); // TODO: remove debugging
		
		/*
		MOTOR_set_dir(msg.data[0]);
		MOTOR_set_vel(msg.data[1]);
		SERVO_set_pos(msg.data[2]);
		if(msg.data[3]) { SOLENOID_shoot(); }
		*/
		
		
		switch(g_motor_mode)
		{
			case no_control:
				MOTOR_set_dir(msg.data[0]);
				MOTOR_set_vel(msg.data[1]);
				SERVO_set_pos(msg.data[2]);
				if(msg.data[3]) { SOLENOID_shoot(); }
				break;
			case pid_position_control:
				MOTOR_set_position(msg.data[2]);
				break;
		}
	
		/*
		if (IR_beam_broken())
		{
			can_message_t msg = {.id=CAN_MSG_IR_DETECTION, .length=0};
			CAN_message_send(&msg);
			printf("IR broken \r\n");
		}
		*/
		_delay_ms(100);
	}
}


void on_incoming_CAN(can_message_t * msg)
{
	if (msg->id == CAN_MSG_MULTI_CMD)
	{
		g_multi_msg = msg;
	}
}