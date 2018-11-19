#include "can.h"
#include "uart.h"
#include "MCP2515.h"
#include "mcp.h"
#include <avr/interrupt.h>

enum interrupt_flags {no_flag, RX0, RX1};
enum interrupt_flags interrupt_flag = no_flag;


bool CAN_get_latest_msg(can_msg_t *msg)
{
	switch(interrupt_flag)
	{
		case no_flag:
			return false;
		case RX0:
			CAN_msg_receive(msg, MCP_RXB0CTRL);
			interrupt_flag = no_flag;
			return true;
		case RX1:
			CAN_msg_receive(msg, MCP_RXB1CTRL);
			interrupt_flag = no_flag;
			return true;
		default:
			break;
	}
	return false;
}

void CAN_print(can_msg_t *msg)
{
	printf("[CAN] .id: %d, length: %d data: [ ", msg->id, msg->length);
	for (int i = 0; i < msg->length; i++)
	{
		printf("%03d ", msg->data[i]);
	}
	printf("]\r\n");
}

void CAN_init(can_mode_t mode){
	MCP_init(0);
	// enable interrupt on CAN controller (enable CANINTE.RXnIE)
	MCP_write(MCP_CANINTE, MCP_RX_INT);
		
	
	// set up interrupt pins on ATmega	
	#if defined(__AVR_ATmega162__)
	// the falling edge of INT3 generates an interrupt request
	MCUCR |= (1 << ISC01) | (0 << ISC00);
	GICR |= (1 << INT0);
	#endif

	#if defined(__AVR_ATmega2560__)
	// the falling edge of INT3 generates an interrupt request
	EICRA |= (1 << ISC31) | (0 << ISC30);
	// Enable external interrupts of INT3
	EIMSK |= (1 << INT3);
	printf("initialized atmega2560 CAN\r\n");
	#endif
	
}

void CAN_send_msg(can_msg_t* msg){
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

void CAN_msg_receive(can_msg_t *msg, uint8_t reg)
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

can_msg_type_t	CAN_get_msg_type(can_msg_t *msg)
{
	return msg->id;
}

// register interrupt handlers for incoming CAN messages
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