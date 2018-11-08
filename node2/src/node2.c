#if 1

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

void nop_handler(can_message_t *msg){};

void can_msg_handler(can_message_t *msg)
{	
	switch(msg->id)
	{
		case  CAN_MSG_SERVO_CMD:
			SERVO_set_pos(255 - msg->data[0]);
			break;
		case CAN_MSG_MOTOR_CMD:
			MOTOR_set_dir(msg->data[0]);
			MOTOR_set_velocity(msg->data[1]);
			break;
		case CAN_MSG_SOLENOID_CMD:
			SOLENOID_shoot();
			break;
	}
}

int main()
{
	cli();
	UART_Init(UBRR);
	SPI_init();
	CAN_init(0);
	CAN_set_receive_handler(nop_handler);
	PWM_init();
	IR_init();
	MOTOR_init();
	SOLENOID_init();
	sei();

	MOTOR_find_limits();
	CAN_set_receive_handler(can_msg_handler);
	//MOTOR_start_controller();


	printf("init done\r\n");
	
	can_message_t msg;
	int i = 0;
	while(1)
	{	
		/*
		msg.id = 3; msg.length = 3; msg.data[0] = 69;
		
		CAN_handle_interrupt(&msg);	
		if (msg.id == CAN_MSG_SERVO_CMD) 
		{

			uint8_t servo_pos = 255 - msg.data[0];
			//printf("got CAN_MSG_SERVO_CMD with data %d\r\n",servo_pos);
			SERVO_set_pos(servo_pos);
		}
		else if (msg.id == CAN_MSG_MOTOR_CMD)
		{
			printf("got CAN_MSG_MOTOR_CMD with direction %d and speed %d \r\n", msg.data[0], msg.data[1]);
			//MOTOR_set_position(msg.data[1]);
			MOTOR_set_dir(msg.data[0]);
			MOTOR_set_velocity(msg.data[1]);
		}
		
		if (IR_beam_broken()){
			printf("IR beam is broken\r\n");
		} else {
			printf("IR beam is NOT broken\r\n");
		}
		*/
		
		_delay_ms(1000);
		printf("\n");
	}
}

#endif