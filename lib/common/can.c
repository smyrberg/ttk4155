#include "can.h"
#include "MCP2515.h"
#include "uart.h"
#include <avr/interrupt.h>

enum interrupt_flags {no_flag, RX0, RX1};
enum interrupt_flags interrupt_flag = no_flag;
#define CAN_NO_MESSAGE	0x0A	//10


void CAN_init(can_mode_t mode)
{
	MCP_init();
	// Turn mask/filters off
	MCP_modify_bit(MCP_RXB0CTRL, MCP_FILTER_OFF, MCP_FILTER_OFF);
	MCP_modify_bit(MCP_RXB1CTRL, MCP_FILTER_OFF, MCP_FILTER_OFF);
	
	// Rollover enable
	MCP_modify_bit(MCP_RXB0CTRL, MCP_ROLLOVER, MCP_ROLLOVER);
	MCP_modify_bit(MCP_RXB1CTRL, MCP_ROLLOVER, MCP_ROLLOVER);
	
	// Set to normal mode
	MCP_modify_bit(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);
	
	uint8_t value = MCP_read(MCP_CANSTAT);
	if ((value & MODE_MASK) != MODE_NORMAL){
		return;
	}
	
	// Interrupt pin (enable CANINTE.RXnIE)
	MCP_write(MCP_CANINTE, MCP_RX_INT);
	
	#if defined(__AVR_ATmega162__)
	// Low INT0 generates interrupt request
	MCUCR |= (0 << ISC01) | (0 << ISC00);
	GICR |= (1 << INT0);
	#endif

	#if defined(__AVR_ATmega2560__)
	// Low INT2 generates interrupt request
	EICRA |= (0 << ISC21) | (0 << ISC20);
	// Enable external interrupts of INT2
	EIMSK |= (1 << INT2);
	#endif
}

void CAN_message_send(can_msg_t *message)
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

bool CAN_get_latest_msg(can_msg_t *msg)
{
	switch(interrupt_flag){
		case no_flag:
			msg->data[0] = CAN_NO_MESSAGE;
			return true;
		case RX0:
			CAN_msg_receive(msg, MCP_RXB0CTRL);
			interrupt_flag = no_flag;
			return true;
		case RX1:
			CAN_msg_receive(msg, MCP_RXB1CTRL);
			interrupt_flag = no_flag;
			break;
		default:
			break;
	}
	return false;
}


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
ISR(INT2_vect){
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




void CAN_print(can_msg_t *msg)
{
	printf("[CAN] .id: %d, length: %d data: [ ", msg->id, msg->length);
	for (int i = 0; i < msg->length; i++)
	{
		printf("%03d ", msg->data[i]);
	}
	printf("]\r\n");
}

can_msg_type_t	CAN_get_msg_type(can_msg_t *msg)
{
	CAN_print(msg);
	return msg->id;
}



#if 0

#include "can.h"
#include "uart.h"
#include "MCP2515.h"
#include "mcp.h"
#include <avr/interrupt.h>

// internal enums and functions
enum interrupt_flags {no_flag, RX0, RX1};
enum interrupt_flags interrupt_flag = no_flag; 
static void read_msg_from_register(can_msg_t *msg, uint8_t reg);


void CAN_init(can_mode_t mode)
{
	MCP_init(mode);
	
	// enable interrupt on CAN controller (enable CANINTE.RXnIE)
	MCP_write(MCP_CANINTE, MCP_RX_INT);
		
	// set up interrupt pins on ATmega	
	#if defined(__AVR_ATmega162__)
	// the falling edge of INT3 generates an interrupt request
	MCUCR |= (1 << ISC01) | (0 << ISC00);
	GICR |= (1 << INT0);
	printf("[CAN] initialization done (ATmega162)\r\n");
	#endif

	#if defined(__AVR_ATmega2560__)
	// the falling edge of INT3 generates an interrupt request
	EICRA |= (1 << ISC31) | (0 << ISC30);
	// enable external interrupts of INT3
	EIMSK |= (1 << INT3);
	printf("[CAN] initialization done (ATmega2560)\r\n");
	#endif
	
}

bool CAN_get_latest_msg(can_msg_t *msg)
{
	switch(interrupt_flag)
	{
		case no_flag:
			return false;
		case RX0:
			read_msg_from_register(msg, MCP_RXB0CTRL);
			interrupt_flag = no_flag;
			return true;
		case RX1:
			read_msg_from_register(msg, MCP_RXB1CTRL);
			interrupt_flag = no_flag;
			return true;
	}
	return false;
}

void CAN_message_send(can_msg_t* msg)
{
	// NOTE: we only use the TXB0 buffer
	
	// loop until TXREQ is cleared  
	uint8_t status , txreq;
	do {
		status = MCP_read_status();
		txreq = (status & 4);

	} while(txreq);

	// set message ID, standard mode 
	MCP_write(MCP_TXB0CTRL + 1, 0);					// high level identifier
	MCP_write(MCP_TXB0CTRL + 2, (msg->id << 5));	// low level identifier
	
	// set data length  
	MCP_write(MCP_TXB0CTRL + 5, msg->length & 0x0F);
	
	// load message data  
	for (uint8_t i = 0; i < msg->length; i++) 
	{
		MCP_write(MCP_TXB0CTRL + 6 + i, msg->data[i]);
	}
	
	// request to send contents of TXB0  
	MCP_request_to_send(MCP_RTS_TX0);
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

can_msg_type_t	CAN_get_msg_type(can_msg_t *msg)
{
	return msg->id;
}


// interrupt pin on different nodes
#if defined(__AVR_ATmega162__)
	#define INTERRUPT_PIN INT0_vect
#endif
#if defined(__AVR_ATmega2560__)
	#define INTERRUPT_PIN INT2_vect
#endif

// define interrupt handler
ISR(INTERRUPT_PIN){
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





static void read_msg_from_register(can_msg_t *msg, uint8_t reg)
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

#endif
