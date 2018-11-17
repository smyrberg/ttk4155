#include "joystick.h"
#include "adc.h"

#include "common/bitmanip.h"

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

int JOY_right_button(){
	return (test_bit(PINB, PINB2));
}


JOY_position_t JOY_get_position(){
	JOY_position_t current_pos = JOY_read_adc();
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



menu_direction_t JOY_get_4axis_direction()
{
	JOY_position_t pos = JOY_get_position();
	if (pos.y > 240)
	{
		while(JOY_get_position().y > 240);
		return menu_up;
	}
	if (pos.y <  15)
	{
		while(JOY_get_position().y < 15);
		return menu_down;
	}
	if (pos.x > 240)
	{
		while(JOY_get_position().x > 240);
		return menu_right;
	}
	if (pos.x < 15 )
	{
		while(JOY_get_position().x < 15);
		return menu_left;
	}