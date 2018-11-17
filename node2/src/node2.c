#include <avr/interrupt.h>
#include <avr/delay.h>
#include "common/can.h"
#include "common/uart.h"
#include "common/spi.h"
#include "drivers/servo.h"
#include "drivers/ir.h"
#include "drivers/motor.h"
#include "drivers/solenoid.h"



#if 0
int main()
{
	cli();
	UART_Init(UBRR);
	printf("starting\r\n");
	CAN_init(CAN_normal_mode);
	sei();
	
	
	printf("entering looop\r\n");
	can_msg_t msg;
	while(1)
	{
		if(CAN_get_latest_msg(&msg))
		{
			CAN_print(&msg);
		}
	}
}

#endif



int main()
{
	cli();
	UART_Init(UBRR);
	SERVO_init();
	IR_init();
	MOTOR_init();
	MOTOR_find_limits();
	SOLENOID_init();
	CAN_init(CAN_normal_mode);
	sei();
	printf("[NODE2] INFO: all initialization done\r\n");
	
	
	uint8_t current_ir = 0, previous_ir = 0;
	can_msg_t msg;
	while(1)
	{
		
		bool got_message = CAN_get_latest_msg(&msg);
		if (got_message)
		{
			CAN_print(&msg);
		
			switch(CAN_get_msg_type(&msg))
			{
				case CAN_msg_set_mode:
					MOTOR_set_mode(msg.data[0]);
					break;
				case CAN_msg_command:
					if (MOTOR_get_mode() == MOTOR_mode_pid)
					{
						MOTOR_set_position(msg.data[0]);
					}
					else if (MOTOR_get_mode() == MOTOR_mode_no_ctrl)
					{
						MOTOR_set_speed(msg.data[0]);
						MOTOR_set_direction(msg.data[1]);
					}
					
					SERVO_set_position(msg.data[2]);
					if (msg.data[3])
					{
						//SOLENOID_shoot();
					}
					break;
			}
		
		}
		
		
		
		if (MOTOR_get_mode() == MOTOR_mode_pid)
		{
			MOTOR_pid_update();
		}
		
		
		current_ir = IR_beam_broken();
		uint8_t ir_has_been_broken = (current_ir - previous_ir != 0) && current_ir;
		if (ir_has_been_broken)
		{
			//printf("[NODE2] INFO: IR has been broken\r\n");
			can_msg_t m = {.id=CAN_msg_ir, .length=1, .data={1}};
			CAN_message_send(&m);
		}
		
	}
}