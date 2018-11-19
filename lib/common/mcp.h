/*!
 * @file mcp.h
 * @brief Mid level driver for CAN controller.
 */
#ifndef MCP_H_
#define MCP_H_

#include <avr/io.h>
#include <stdlib.h>
#include "spi.h"
#include "MCP2515.h"

/*!
 * Initialize the CAN controller.
 */
void MCP_init(int in_loopback);

/*!
 * Reset the CAN controller.
 */
void MCP_reset();

/*!
 * Read contents for register
 * @param[in] addr Address to read from
 * @return read data
 */
uint8_t MCP_read(uint8_t addr);

/*!
 * Write to register
 * @param[in] addr Destination address.
 * @param[in] data Data to write.
 */
void MCP_write(uint8_t addr, uint8_t data);

/*!
 * Read status of rx/tx.
 * @return status.
 */
uint8_t MCP_read_status();

/*!
 * Set the mode of operation of the MCP.
 * @param[in] mode Mode to set.
 */
void MCP_set_mode(uint8_t mode);

/*!
 * Change the value of register with specified mask.
 * @param[in] addr Address of register.
 * @param[in] mask Bitmask to specifiy what bits to modify
 * @param[in] data New bits to set.
 */
void MCP_modify_bit(uint8_t addr, uint8_t mask, uint8_t data);

/*!
 * Ask controller to begin transmission of buffer.
 * @param[in] buffer Buffer to send.
 */
void MCP_request_to_send(uint8_t buffer);

/*!
 * Get content of buffer. Reduced overhead compared to MCP_read.
 * @param[in] buffer Buffer to read from.
 * @return Data read
 */
uint8_t MCP_read_rx_buffer(uint8_t buffer);

/*!
 * Load transmit buffer with data. Reduced overhead compared to MCP_write.
 * @param[in] buffer Buffer to transmit.
 * @param[in] data Data to transmit.
 */
void MCP_load_tx_buffer(uint8_t buffer, uint8_t data);

/*!
 * Get info whether a message is in the receive buffer(s), message type and filter match.
 * @return Status of message buffers.
 */
uint8_t MCP_rx_status();
	

#endif /* MCP_H_ */