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
	printf("init done\n\r");
	sei();
	can_message_t send = {.id = 3, .length = 3, .data = 69};
	can_message_t msg;

	/* // TX
	int i = 0;
	while(1){

		CAN_message_send(&send);
		printf("\n\n=== NODE 1 === \n\r");
		printf("iteration %d\r\n", ++i);
		printf("Sent message: %d \r\n",send.data[0]);
		printf("Sent id: %d \r\n", send.id);
		printf("Sent length: %d  \r\n", send.length);
		
		_delay_ms(3000);
		
		
	}
	*/
	
	// RX
	printf("\n\n=== NODE 1 === \n\r");
	int i = 0;
	while(1){
		
		CAN_handle_interrupt(&msg);
		if (msg.data[0] == CAN_NO_MESSAGE){
			printf("\rsec: %d no data...                           ", ++i);
			}else{
			printf("\rsec: %d rx data: %d id: 0x%02x len: 0x%02x",++i,  msg.data[0], msg.id, msg.length);
		}
		
		_delay_ms(1000);
	}

	return 0;
}