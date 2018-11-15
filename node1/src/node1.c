#include "common/spi.h"
#include "common/can.h"
#include "common/uart.h"

#include "drivers/joystick.h"
#include "drivers/oled.h"

#include "menu.h"

#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>

void main()
{	
	cli();
	UART_Init(UBRR);
	JOY_init();
	OLED_init();
	SPI_init();
	CAN_init(0);
	MENU_init();
	sei();
	
	MENU_main();
	return;	
}