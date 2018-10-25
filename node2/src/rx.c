#if 0

#include "../../common/uart.h"
#include "../../common/spi.h"
#include "../../common/MCP2515.h"
#include "../../common/can.h"

#define F_CPU 16000000L
#define FOSC 16000000	// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>




int main( void ){
	cli();
	// Initializations
	UART_Init(MYUBRR);
	printf("node 2\r\n");
	SPI_init();
	CAN_init(0);
	sei();
	can_message_t msg = {.id = 5, .length = 9, .data = 99};
	
	printf("\n\n=== NODE 2 === \n\r");
	/* // RX
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
	*/
	
	can_message_t send = {.id = 3, .length = 3, .data = 69};
	// TX
	int i = 0;
	while(1){

		CAN_message_send(&send);
		printf("\n\n=== NODE 2 === \n\r");
		printf("iteration %d\r\n", ++i);
		printf("Sent message: %d \r\n",send.data[0]);
		printf("Sent id: %d \r\n", send.id);
		printf("Sent length: %d  \r\n", send.length);
		
		_delay_ms(3000);
		
		
	}
		
	
	return 0;
}
#endif