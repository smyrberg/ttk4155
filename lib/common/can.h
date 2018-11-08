#ifndef CAN_H_
#define CAN_H_

#include "MCP2515.h"
#include "mcp.h"


// message id
#define CAN_MSG_SERVO_CMD 1 // data[0] = position (range 0-255)
#define CAN_MSG_MOTOR_CMD 2 // data[0] = direction (0=right, 1=left), data[1] = speed (range 0-255)
#define CAN_MSG_SOLENOID_CMD 3 // no data

typedef struct {
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
} can_message_t;

typedef void (*CAN_msg_handler_t) (can_message_t*);

enum interrupt_flags {no_flag, RX0, RX1};
#define CAN_NO_MESSAGE	0x0A	//10

void CAN_init(int in_loopback);
void CAN_message_send(can_message_t* msg);
can_message_t CAN_message_receive(void);

void CAN_msg_send(can_message_t *message);
void CAN_msg_receive(can_message_t *msg, uint8_t reg);
void CAN_handle_interrupt(can_message_t *msg);

void CAN_set_receive_handler(CAN_msg_handler_t handler);


#endif /* CAN_H_ */