#include "can.h"

void CAN_init(int in_loopback){
	MCP_init(in_loopback);
	
	// Interrupt pin (enable CANINTE.RXnIE)
	MCP_write(MCP_CANINTE, MCP_RX_INT);
		
	#if defined(__AVR_ATmega162__)
	// Falling edge of INT0 generates interrupt request
	MCUCR |= (1 << ISC01) | (0 << ISC00);
	GICR |= (1 << INT0);
	#endif

	#if defined(__AVR_ATmega2560__)
	// Falling edge of INT2 generates interrupt request
	EICRA |= (1 << ISC21) | (0 << ISC20);
	// Enable external interrupts of INT2
	EIMSK |= (1 << INT2);
	#endif
	
}

/* High level function for sending a message on the CAN bus */
void CAN_message_send(can_message_t* msg){
	/* Only transmit buffer TXB0 is used */
	
	/* Loop until TXREQ is cleared */
	uint8_t status , txreq;
	do {
		status = MCP_read_status();
		txreq = (status & 4);

	} while(txreq);

	/* Set message ID, standard mode*/
	MCP_write(MCP_TXB0CTRL + 1, 0); // High level identifier
	MCP_write(MCP_TXB0CTRL + 2, (msg->id << 5)); // Low level identifier
	
	/* Set data length */
	MCP_write(MCP_TXB0CTRL + 5, msg->length & 0x0F);
	
	/* Load message data */
	for (uint8_t i = 0; i < msg->length; i++) {
		MCP_write(MCP_TXB0CTRL + 6 + i, msg->data[i]);
	}
	
	/* Request to send contents of TXB0 */
	MCP_request_to_send(MCP_RTS_TX0);
}

can_message_t CAN_message_receive(void){
	/* Only RXB0 is used */
	
	can_message_t message;
	uint8_t buffer = MCP_read(MCP_RXB0SIDH + 1);
	message.id = (buffer >> 5);
	buffer = MCP_read(MCP_RXB0CTRL + 5);
	message.length = (buffer & 0x0F);
	for (uint8_t i = 0; i < message.length; i++) {
		message.data[i] = MCP_read(MCP_RXB0CTRL + 6 + i);
	}
	
	/* Clear CANINTF.RX0IF */
	MCP_modify_bit(MCP_CANINTF, 0x01, 0x00);
	
	uint8_t canintf = MCP_read(MCP_CANINTF);
	
	return message;
}