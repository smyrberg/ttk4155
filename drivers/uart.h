#ifndef UART_H
#define UART_H


void UART_Init( unsigned int ubrr );
void UART_Transmit (unsigned char data);
unsigned char UART_Recieve (void);

#endif