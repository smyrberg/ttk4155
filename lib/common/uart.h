#ifndef UART_H
#define UART_H

#if defined(__AVR_ATmega162__)
	#define F_CPU 4915200
	#define FOSC 4915200// Clock Speed
	#define BAUD 9600
	#define UBRR FOSC/16/BAUD-1
#endif

#if defined(__AVR_ATmega2560__)
	#define F_CPU 16000000L
	#define FOSC 16000000	// Clock Speed
	#define BAUD 9600
	#define UBRR FOSC/16/BAUD-1
#endif

extern int printf(const char *__fmt, ...);
void UART_Init(unsigned int ubrr );
void UART_Transmit(unsigned char data);
unsigned char UART_Recieve(void);

#endif