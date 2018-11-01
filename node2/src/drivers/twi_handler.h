#ifndef TWI_HANDLER_H_
#define TWI_HANDLER_H_

#include <avr/io.h>

#define TWI_TARGET_SLAVE_ADDRESS	0x50

unsigned char TWI_act_on_failure_in_last_transmission( unsigned char TWIerrormsg );
void TWI_send_address_and_data(uint8_t *message, int message_length);

#endif /* TWI_HANDLER_H_ */