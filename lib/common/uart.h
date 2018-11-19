/*!
 * @file
 * @brief UART driver to communicate with computer over serial port.
 * This file will also define F_CPU correctly for each node.
 */
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

/*!
 * Print formatted strings to the console.
 * @param[in] __fmt The string to print containing formatting directives.
 * @param[in] ... Variables for the formatting directives.
 * @return Number of characters printed.
 */
extern int printf(const char *__fmt, ...);

/*!
 * Initializes the UART driver.
 * @param[in] ubrr Baud rate. (easiest to use UBRR which should be correctly defined)
 */
void UART_Init(unsigned int ubrr);

/*!
 * Transmit one byte to UART.
 * @param[in] data The byte to send.
 */
void UART_Transmit(unsigned char data);

/*!
 * Receive one byte from UART.
 * @return Byte received.
 */
unsigned char UART_Recieve();

#endif