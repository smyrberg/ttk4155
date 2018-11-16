#ifndef CAN_H_
#define CAN_H_

#include <stdbool.h>
#include <stdio.h>

typedef enum can_msg_type_t{
	CAN_msg_set_mode,
	CAN_msg_command,
	CAN_msg_ir
} can_msg_type_t;

typedef struct {
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
} can_msg_t;

typedef enum can_mode_t {
	CAN_normal_mode = 0,
	CAN_loopback_mod
} can_mode_t;

void CAN_init(can_mode_t mode);
void CAN_message_send(can_msg_t *msg);
bool CAN_get_latest_msg(can_msg_t *msg);
can_msg_type_t	CAN_get_msg_type(can_msg_t *msg);
void CAN_print(can_msg_t *msg);


#endif /* CAN_H_ */