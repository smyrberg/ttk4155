#include "can.h"
#include "uart.h"
#include <avr/interrupt.h>

enum interrupt_flags interrupt_flag = no_flag; 

void CAN_init(int in_loopback){
	MCP_init(in_loopback);
	
	// enable interrupt on CAN controller (enable CANINTE.RXnIE)
	MCP_write(MCP_CANINTE, MCP_RX_INT);
	
	
	
	
	// set up interrupt pins on ATmega	
	#if defined(__AVR_ATmega162__)
	// the falling edge of INT3 generates an interrupt request
	MCUCR |= (1 << ISC01) | (0 << ISC00);
	GICR |= (1 << INT0);
	printf("initialized atmega162 CAN\r\n");
	#endif

	#if defined(__AVR_ATmega2560__)
	// the falling edge of INT3 generates an interrupt request
	EICRA |= (1 << ISC31) | (0 << ISC30);
	// Enable external interrupts of INT3
	EIMSK |= (1 << INT3);
	printf("initialized atmega2560 CAN\r\n");
	#endif
	
}

void CAN_msg_send(can_message_t *message)
{
	// Write ID to TXB0SIDH
	MCP_write(MCP_TXB0SIDH, (message->id) >> 3);
	// Write 0 to TXB0SIDL and extended identifier registers
	MCP_write(MCP_TXB0SIDL, (message->id) << 5);
	MCP_write(MCP_TXB0EID8, 0);
	MCP_write(MCP_TXB0EID0, 0);
	
	//Write data length
	MCP_write(MCP_TXB0DLC, message->length);
	
	for (int i = 0; i < message->length; i++){
		MCP_write(MCP_TXB0SIDH + 5 + i, message->data[i] );
	}
	MCP_request_to_send(MCP_RTS_TX0);
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
	printf("tx cleared\r\n");

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

void CAN_msg_receive(can_message_t *msg, uint8_t reg)
{
	// RXBnSIDH and RXBnSIDL (id)
	msg->id = (MCP_read(reg + 1) << 3) | (MCP_read(reg + 2) >> 5);
	// bit 0 to 3 are data length code bits. register + 5 is RXBnDLC (data length)
	msg->length = (MCP_read(reg + 5) & 0x0F );
	
	int i = 0;
	while( (i < msg->length) && (i < 8) ){
		//RXBnDM (receive buffer)
		msg->data[i] = MCP_read(reg + 6 + i);
		i++;
	}
}

void CAN_handle_interrupt(can_message_t *msg)
{
	
	switch(interrupt_flag){
		case no_flag:
			msg->data[0] = CAN_NO_MESSAGE;
			break;
		case RX0:
			CAN_msg_receive(msg, MCP_RXB0CTRL);
			interrupt_flag = no_flag;
			break;
		case RX1:
			CAN_msg_receive(msg, MCP_RXB1CTRL);
			interrupt_flag = no_flag;
			break;
		default:
			break;
	}
}


// register interrupt handlers for incomming CAN messsages
#if defined(__AVR_ATmega162__)
ISR(INT0_vect){
	uint8_t interrupt = MCP_read(MCP_CANINTF);

	if (interrupt & MCP_RX0IF){
		interrupt_flag = RX0;
		// clear CANINTF.RX0IF
		MCP_modify_bit(MCP_CANINTF, 0x01, 0x00);
	}
	else if (interrupt & MCP_RX1IF){
		interrupt_flag = RX1;
		// clear CANINTF.RX1IF
		MCP_modify_bit(MCP_CANINTF, 0x02, 0x00);
	}
}
#endif

#if defined(__AVR_ATmega2560__)
ISR(INT3_vect){
	uint8_t interrupt = MCP_read(MCP_CANINTF);

	if (interrupt & MCP_RX0IF){
		interrupt_flag = RX0;
		// clear CANINTF.RX0IF
		MCP_modify_bit(MCP_CANINTF, 0x01, 0x00);
	}
	else if (interrupt & MCP_RX1IF){
		interrupt_flag = RX1;
		// clear CANINTF.RX1IF
		MCP_modify_bit(MCP_CANINTF, 0x02, 0x00);
	}
}
#endif