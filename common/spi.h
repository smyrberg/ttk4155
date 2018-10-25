#ifndef SPI_H_
#define SPI_H_

#define SPI_CS_ENABLE 0
#define SPI_CS_DISABLE 1

#include <avr/io.h>

void SPI_init();
void SPI_MasterTransmit(uint8_t data);

uint8_t SPI_transmit_receive(uint8_t data);
void SPI_set_ss(int val);

#endif /* SPI_H_ */