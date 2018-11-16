#include "mcp.h"
#include "uart.h"

void MCP_init(int in_loopback)
{
	// SPI_init();
	MCP_reset();
	
	// turn mask/filters off (receive any message) and also enable rollover
	// 0x64 = 0b01100100
	MCP_modify_bit(MCP_RXB0CTRL, 0x64, 0x64);
	MCP_modify_bit(MCP_RXB1CTRL, 0x64, 0x64);
	
	
	// set mode for controller
	if(in_loopback){
		printf("[CAN] MCP in loopback mode\r\n");
		MCP_set_mode(MODE_LOOPBACK); // Sets loopback operation mode for testing
	} else {
		printf("[CAN] MCP in normal mode\r\n");
		MCP_set_mode(MODE_NORMAL); // Sets normal operation mode
	}
}

void MCP_reset(void)
{
	SPI_set_ss(SPI_CS_ENABLE);
	SPI_transmit_receive(MCP_RESET);
	SPI_set_ss(SPI_CS_DISABLE);
}

// Returns the content of a register
uint8_t MCP_read(uint8_t addr)
{
	SPI_set_ss(SPI_CS_ENABLE);
	SPI_transmit_receive(MCP_READ);
	SPI_transmit_receive(addr);
	SPI_transmit_receive(0x00);
	char data = SPDR;
	SPI_set_ss(SPI_CS_DISABLE);
	return data;
}

void MCP_write(uint8_t addr, uint8_t data)
{
	SPI_set_ss(SPI_CS_ENABLE);
	SPI_transmit_receive(MCP_WRITE);
	SPI_transmit_receive(addr);
	SPI_transmit_receive(data);
	SPI_set_ss(SPI_CS_DISABLE);
}

// Returns the content of the specified buffer. Reduces overhead of MCP_read
uint8_t MCP_read_rx_buffer(uint8_t buffer)
{
	SPI_set_ss(SPI_CS_ENABLE);
	SPI_transmit_receive(buffer);
	SPI_transmit_receive(0x00);
	char data = SPDR;
	SPI_set_ss(SPI_CS_DISABLE);
	return data;
}

// Loads the specified transmit buffer with data. Reduces overhead of MCP_write
void MCP_load_tx_buffer(uint8_t buffer, uint8_t data)
{
	SPI_set_ss(SPI_CS_ENABLE);
	SPI_transmit_receive(buffer);
	SPI_transmit_receive(data);
	SPI_set_ss(SPI_CS_DISABLE);
}

// Instructs controller to begin message transmission for the selected buffers
void MCP_request_to_send(uint8_t buffer)
{
	SPI_set_ss(SPI_CS_ENABLE);
	SPI_transmit_receive(buffer);
	SPI_set_ss(SPI_CS_DISABLE);
}

// Returns some status bits for transmit and receive functions 
uint8_t MCP_read_status(void)
{
	SPI_set_ss(SPI_CS_ENABLE);
	SPI_transmit_receive(MCP_READ_STATUS);
	SPI_transmit_receive(0x00);
	char status = SPDR;
	SPI_set_ss(SPI_CS_DISABLE);
	return status;
}

// Returns info about whether a message is in the receive buffer(s), message type and filter match
uint8_t MCP_rx_status(void)
{
	SPI_set_ss(SPI_CS_ENABLE);
	SPI_transmit_receive(MCP_RX_STATUS);
	char status = SPDR;
	SPI_set_ss(SPI_CS_DISABLE);
	return status;
}

// Set MCP mode of operation
void MCP_set_mode(uint8_t mode)
{
	uint8_t mask = 0b11100000;
	MCP_modify_bit(MCP_CANCTRL, mask, mode);
}

// Changes the value of the register bits specified by the mask 
void MCP_modify_bit(uint8_t addr, uint8_t mask, uint8_t data)
{
	SPI_set_ss(SPI_CS_ENABLE);
	SPI_transmit_receive(MCP_BITMOD);
	SPI_transmit_receive(addr);
	SPI_transmit_receive(mask);
	SPI_transmit_receive(data);
	SPI_set_ss(SPI_CS_DISABLE);
}