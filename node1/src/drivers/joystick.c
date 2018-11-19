#include "joystick.h"
#include "adc.h"
#include "common/bitmanip.h"
#include <util/delay.h>

#define X_POS_CHANNEL 0
#define Y_POS_CHANNEL 1

JOY_position_t neutral_pos;


static void calibrate(){
	neutral_pos = JOY_get_position();
}

void JOY_init(){
	ADC_init();
	calibrate();

	// initialize PORT B, pin 0, 1 and 2 as input
	clear_bit(DDRB, PINB0);
	clear_bit(DDRB, PINB1);
	clear_bit(DDRB, PINB2);

	// activate pull-up resistor for joystick button
	set_bit(PORTB, PB0);
}

int JOY_right_button(){
	return (test_bit(PINB, PINB2));
}

JOY_position_t JOY_get_position(){
	JOY_position_t pos;
	pos.y = ADC_read(Y_POS_CHANNEL);
	_delay_ms(1);
	pos.x = ADC_read(X_POS_CHANNEL);
	return pos;
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
}