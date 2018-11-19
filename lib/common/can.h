/*!
 * @file can.h
 * @brief High level CAN driver.
 */
#ifndef CAN_H_
#define CAN_H_

#include <stdbool.h>
#include <stdio.h>

/*!
 * Messages for transmission over the CAN bus.
 */
typedef struct {
	unsigned int id;		/*!< Identifier specifying message type.*/
	uint8_t length;			/*!< Length of data.*/
	uint8_t data[8];		/*!< Data buffer.*/
} can_msg_t;

/*!
 * Enumeration of different CAN message types.
 */
typedef enum can_msg_type_t{
	CAN_msg_set_mode,
	CAN_msg_command,
	CAN_msg_ir
} can_msg_type_t;

/*!
 * Enumeration modes available for the CAN controller.
 */
typedef enum can_mode_t {
	CAN_normal_mode = 0,
	CAN_loopback_mode =1
} can_mode_t;

/*!
 * Initializes the CAN bus.
 * @param[in] mode The operating mode for the driver.
 */
void CAN_init(can_mode_t mode);

/*!
 * Send a message onto the CAN bus.
 * @param[in] msg The message to send.
 */
void CAN_send_msg(can_msg_t* msg);

/*!
 * Retrieve the latest message in the message buffer
 * @param[out] msg The message to copy any new messages into.
 * @return Return true if a message was retrieved, otherwise false.
 */
bool CAN_get_latest_msg(can_msg_t *msg);

/*!
 * Print a CAN message.
 * Format: [CAN] .id: \<id\> .length: \<length\> .data: [\<data0\> \<data1\> ... ]
 * @param[in] msg The message to print.
 */
void CAN_print(can_msg_t *msg);

/*!
 * Get the message type of message.
 * @param[in] msg The message to get the type of.
 * @return The type of the message.
 */
can_msg_type_t CAN_get_msg_type(can_msg_t *msg);

#endif /* CAN_H_ */