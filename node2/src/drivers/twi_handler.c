#include "TWI_Master.h"
#include "twi_handler.h"

unsigned char TWI_act_on_failure_in_last_transmission( unsigned char TWIerrormsg )
{
	if (( TWIerrormsg == TWI_MTX_ADR_NACK ) | ( TWIerrormsg == TWI_MRX_ADR_NACK )){
		TWI_Start_Transceiver();
	}
	return TWIerrormsg;
}

void TWI_send_address_and_data(uint8_t *message, int message_length)
{
	uint8_t message_buffer[4];
	
	message_buffer[0] = TWI_TARGET_SLAVE_ADDRESS;
	message_buffer[1] = 0x00;
	for (int i = 0; i < message_length; i++){
		message_buffer[i+2] = message[i];
	}
	TWI_Start_Transceiver_With_Data(message_buffer, message_length + 2);
}