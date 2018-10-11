#include "../../drivers/uart.h"
#include "../../drivers/spi.h"
#include "../../drivers/MCP2515.h"
#include "../../drivers/can.h"

#include <avr/io.h>
#include <util/delay.h>
//#include <stdio.h>
#include <avr/interrupt.h>

int main( void ){
	cli();
	// Initializations
	UART_Init(UBRR);
	
	SPI_init();
    CAN_init(0);
	printf("\n\n\nInit done\n\r");
	sei();
	can_message_t send = {.id = 1, .length = 1, .data = 69};
	
	int i = 1;
	while(1){
		CAN_message_send(&send);
		printf("\n\niteration %d\r\n", i);
		printf("Sent message: %d \r\n",send.data[0]);
		printf("Sent id: %d \r\n", send.id);
		printf("Sent length: %d  \r\n", send.length);
		
		_delay_ms(500);
		
		
	}
	
	

	return 0;
}