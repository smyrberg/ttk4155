#if 1

#include "drivers/servo.h"
#include "drivers/pwm.h"
#include "drivers/ir.h"
#include "drivers/adc.h"

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
	ADC_init();
	sei();
	
	can_message_t msg;
	int i = 0;
	while(1){
		CAN_handle_interrupt(&msg);	
		if (msg.id == CAN_MSG_SERVO_CMD) {
			SERVO_set_pos(msg.data[0]);
		}
		
		int ir_val = IR_read();
		printf("IR value: %d\n\r", ir_val);
		
		_delay_ms(1000);
	}
}

#endif