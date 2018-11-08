#include "common/spi.h"
#include "common/can.h"
#include "common/uart.h"

#include "drivers/joystick.h"

#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>

can_message_t pos2motorcmd(JOY_position_t pos)
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

void main()
{	
	cli();
	UART_Init(UBRR);
	JOY_init();
	SPI_init();
	CAN_init(0);	
	sei();
	
	int shot_disallowed = 0;
	while(1)
	{
		JOY_position_t pos = JOY_get_position();
	
		can_message_t servo_msg = {.id=CAN_MSG_SERVO_CMD, .length=1, .data=pos.y};
		CAN_message_send(&servo_msg);
		//printf("CAN_MSG_SERVO_CMD with data %d\n\r", pos.x);
		//_delay_ms(20);
		can_message_t motor_msg = pos2motorcmd(pos);
		CAN_message_send(&motor_msg);
		//printf("CAN_MSG_MOTOR_CMD with direction %d and speed %d\n\r", motor_msg.data[0], motor_msg.data[1]);
		
		if (shot_disallowed)
		{
			printf("recharging: %d\r\n", shot_disallowed);
			shot_disallowed--;
		} 
		else if (JOY_right_button())
		{
			printf("shoot!\r\n");
			can_message_t solenoid_msg = {.id=CAN_MSG_SOLENOID_CMD, .length=0};
			CAN_message_send(&solenoid_msg);
			shot_disallowed = 15;
		}

		_delay_ms(20);
	}
	
}