/*!
 * @file spi.h
 * @brief Low level SPI driver to communicate with CAN controller.
 */
#ifndef SPI_H_
#define SPI_H_

#define SPI_SS_ENABLE 0		/*!< Enable signal slave*/
#define SPI_SS_DISABLE 1	/*!< Disable signal slave*/

#include <avr/io.h>

/*!
 * Initialize SPI interface.
 */
void SPI_init();


/*!
 * Transmit data to SPI and return values from SPDR register.
 * For transmit operation ignore return value.
 * Read data may also be retrieved from SPDR directly after the function is done.
 * @param[in] data Data to transmit.
 */
uint8_t SPI_transmit_receive(uint8_t data);

/*!
 * Set Select Slave signal.
 * @param[in] val Use SPI_SS_{ENABLE,DISABLE} macros.
 */
void SPI_set_ss(int val);

#endif /* SPI_H_ */