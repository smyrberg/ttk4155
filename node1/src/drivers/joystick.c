#include "joystick.h"
#include "adc.h"

#include "../../../common/bitmanip.h"

#include <util/delay.h>

JOY_position_t neutral_pos;


void JOY_init( void ){
	ADC_init();
	calibrate();

	// initialize PORT B, pin 0, 1 and 2 as input
	clear_bit(DDRB, PINB0);
	clear_bit(DDRB, PINB1);
	clear_bit(DDRB, PINB2);

	// activate pull-up resistor for joystick button
	set_bit(PORTB, PB0);


}

JOY_position_t JOY_read_adc(void){
	JOY_position_t pos;
	pos.y = ADC_read(Y_POS_CHANNEL);
	_delay_ms(1);
	pos.x = ADC_read(X_POS_CHANNEL);
	return pos;
}

void calibrate(void){
	neutral_pos = JOY_read_adc();
}

int JOY_button(){
	return !(test_bit(PINB, PINB0));
}


JOY_position_t JOY_get_position(){
	JOY_position_t current_pos = JOY_read_adc();
	

	/*
	JOY_position_t pos_percentage;
	pos_percentage.y = (current_pos.y - neutral_pos.y) * 100 / neutral_pos.y;
	pos_percentage.x = (current_pos.x - neutral_pos.x) * 100 / neutral_pos.x;
	
	// clamp to +- 100
	if (pos_percentage.x > 100) {
		pos_percentage.x = 100;
		} else if (pos_percentage.x < -100) {
		pos_percentage.x = -100;
	}
	if (pos_percentage.y > 100) {
		pos_percentage.y = 100;
		}else if (pos_percentage.y < -100) {
		pos_percentage.y = -100;
	}
	

	return pos_percentage;
	*/
	return current_pos;
}

// TODO: refactor this shit
JOY_direction_t JOY_get_direction(){
	JOY_position_t current_pos = JOY_get_position();

	if (abs(current_pos.x) < 5 && abs(current_pos.y) < 5) {
		return NEUTRAL;
	}
	if (abs(current_pos.x) > abs(current_pos.y)){
		if (current_pos.x > 0) {
			return RIGHT;
		}
		else {
			return LEFT;
		}
	}
	else {
		if (current_pos.y > 0) {
			return UP;
		}
		else {
			return DOWN;
		}
	}
}
