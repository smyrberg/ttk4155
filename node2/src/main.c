#if 1

#include "drivers/servo.h"
#include "drivers/pwm.h"
#include "drivers/ir.h"
#include "drivers/adc.h"
#include "drivers/motor.h"

#include "../../common/uart.h"
#include "../../common/spi.h"
#include "../../common/can.h"

#include <avr/interrupt.h>
#include <util/delay.h>

int main()
{
	cli();
	UART_Init(UBRR);
	SPI_init();
	CAN_init(0);
	PWM_init();
	IR_init();
	MOTOR_init();
	sei();
	
	MOTOR_find_limits();
	for (;;){
		// IR_read();
		//MOTOR_set_velocity(0x80);
		_delay_ms(20);
		printf("ENCODER: %d\r\n", MOTOR_read_scaled_encoder());
		
	}
	
	can_message_t msg;
	int i = 0;
	while(1){
		CAN_handle_interrupt(&msg);	
		if (msg.id == CAN_MSG_SERVO_CMD) {
			SERVO_set_pos(msg.data[0]);
		}
		
		if (IR_beam_broken()){
			printf("IR beam is broken\r\n");
		} else {
			printf("IR beam is NOT broken\r\n");
		}
		
		_delay_ms(1000);
	}
}

#endif