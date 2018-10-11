#ifndef UART_H
#define UART_H


#define F_CPU 4915200
#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define UBRR FOSC/16/BAUD-1

void UART_Init(unsigned int ubrr );
void UART_Transmit(unsigned char data);
unsigned char UART_Recieve(void);

#endif