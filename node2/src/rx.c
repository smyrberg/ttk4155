#include "../../drivers/uart.h"
#include "../../drivers/spi.h"
#include "../../drivers/MCP2515.h"
#include "../../drivers/can.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define F_CPU 16000000
#define FOSC 16000000	// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

int main( void ){
	cli();
	// Initializations
	UART_Init(MYUBRR);
	printf("node 2\r\n");
	SPI_init();
	CAN_init(0);
	sei();
	can_message_t msg;

	int count = 1;
	while(1){
		printf("\n\niteration %d\r\n", count);

		_delay_us(200);
		msg = CAN_message_receive();
		printf("received message: \r\n",msg.data[0] );
		printf("Received id: %d \r\n", msg.id);
		printf("Received length: %d  \r\n", msg.length);
		_delay_ms(2000);
	}
	
	

	return 0;
}
