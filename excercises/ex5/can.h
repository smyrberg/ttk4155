#ifndef CAN_H_
#define CAN_H_

#include "MCP2515.h"
#include "mcp.h"

typedef struct {
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
	} can_message_t;

void CAN_init(int in_loopback);
void CAN_message_send(can_message_t* msg);
can_message_t CAN_message_receive(void);



#endif /* CAN_H_ */