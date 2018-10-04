#include "spi.h"
#include "uart.h"
#include "can.h"
#include <avr/interrupt.h>
#include <util/delay.h>



void main(){

    UART_Init(UBRR);
    
    SPI_init();
    printf("spi init\r\n");
    CAN_init(1);
    printf("can init\r\n");


    can_message_t send = {.id = 5, .length = 1, .data[0] = 128};
	can_message_t receive;

    for(int i = 1; i <= 10; i++){
        printf("\n\niteration: %d\r\n", i);
        CAN_message_send(&send);
        _delay_ms(1);
        receive = CAN_message_receive();
        printf("tx data: 0x%02x \t rx data: 0x%02x \r\n", send.data[0], receive.data[0]);
		printf("tx id:   0x%02x \t rx id:   0x%02x \r\n", send.id, receive.id);
		printf("tx len:  0x%02x \t rx len:  0x%02x \r\n", send.length, receive.length);
		_delay_ms(2000);



    }
}