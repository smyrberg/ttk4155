#if 1
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

void nop_handler(can_message_t *msg){};

void can_msg_handler(can_message_t *msg)
{	
	switch(msg->id)
	{
		case  CAN_MSG_SERVO_CMD:
			//printf("servo: %d\r\n", msg->data[0]);
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
	//CAN_set_receive_handler(CAN_default_receive_handler);
	CAN_set_receive_handler(can_msg_handler);
	//MOTOR_start_controller();


	printf("init done\r\n");
	
	can_message_t ir_msg;
	uint8_t ir_enable = 1;
	int i = 0;
	while(1)
	{	
		
		if (IR_beam_broken() && ir_enable)
		{
			printf("IR broken\r\n");
			ir_msg = (can_message_t){.id=CAN_MSG_IR_DETECTION, .length=0};
			CAN_message_send(&ir_msg);
			ir_enable = 0;
		}
		else if (!IR_beam_broken())
		{
			ir_enable = 1;
		}
		
		printf("ADC_read() = %d\r\n", ADC_read());
		
		//SOLENOID_shoot();		
		_delay_ms(50);
		//printf("\n");
	}
}

#endif