
#define F_CPU 4915200
#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include "../../drivers/uart.h"
#include <util/delay.h>
#include <stdio.h>


int main( void){
	unsigned char msg = 'x';
	
	UART_Init (MYUBRR);

    printf("\rhello motherfuckers\n");
	
	while(1){
        
        UART_Transmit('K');
        UART_Transmit('U');
        UART_Transmit('K');
        UART_Transmit(':');

        UART_Transmit(msg);

        UART_Transmit('\n');
		UART_Transmit('\r');


        msg = UART_Recieve();



		_delay_ms(100);
	}
	return 0;
}