#include "../../common/spi.h"
#include "../../common/can.h"
#include "../../common/uart.h"

#include "drivers/joystick.h"

#include <util/delay.h>
#include <avr/interrupt.h>

void main()
{	
	cli();
	UART_Init(UBRR);
	JOY_init();
	SPI_init();
	CAN_init(0);	
	sei();
	
	while(1)
	{
		JOY_position_t pos = JOY_get_position();
		printf("x: %d y: %d\r\n", pos.x, pos.y);
		
		can_message_t msg = {.id=CAN_MSG_SERVO_CMD, .length=1, .data=pos.x};
		CAN_message_send(&msg);
		printf("message sent \r\n");
		
		_delay_ms(500);
	}
	
}