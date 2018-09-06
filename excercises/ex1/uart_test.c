#include "uart.h"
#include <util/delay.h>
#include <stdio.h>


int main( void){
	unsigned char msg = 'x';
	
	UART_Init (UBRR);

    printf("\rhello motherfuckers\r\n");
	
	while(1){
        
        UART_Transmit('K');
        UART_Transmit('U');
        UART_Transmit('K');
        UART_Transmit(':');

        UART_Transmit(msg);

        UART_Transmit('\r');
		UART_Transmit('\n');


        msg = UART_Recieve();



		_delay_ms(100);
	}
	return 0;
}