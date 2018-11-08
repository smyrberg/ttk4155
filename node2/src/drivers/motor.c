#include "motor.h"
#include "twi_handler.h"
#include "TWI_Master.h"

#include "common/uart.h"

#define F_CPU 16000000L
#include <util/delay.h>

int16_t MOTOR_max_encoder_value;
volatile uint8_t max_velocity = 0x60;
const uint8_t init_velocity = 0x60;


#include <stdlib.h>
#include <math.h>
#include <avr/interrupt.h>

int Kp = 110;		//1.1*100
int Ki = 10;		//1*10
int Kd = 100;		//Kd * 1000

volatile int run_controller_flag = 0;
volatile uint8_t g_reference = 127;

void MOTOR_start_controller()
{
	// OC3A disconnected. Normal port operation, Normal mode
	
	// Set prescaler to 1/64
	TCCR3B |= (1 << CS31) | (1 << CS30);
	
	// Set the desired output compare match that will generate a timer interrupt
	// Choose dt = 0.1 -> OCR3A = 12500
	OCR3A = 0x30D4;
	
	// Enable output compare interrupt 3A
	TIMSK3 |= (1 << OCIE3A);
}

void MOTOR_set_position(uint8_t reference_pos)
{
	g_reference = reference_pos;
}

int CONTROLLER_set_reference(uint8_t reference)
{	
	g_reference = reference;
	int reff;
	// 255 is rightmost position, 0 is leftmost position
	reff = abs(reference - 0xFF);
	return reff;
}

int CONTROLLER_run(int y, int reference)
{
	static int integral;
	static int u;
	static int prev_err;
	int error;
	int derivative;
	int dt = 1;		//0.1*10
	
	switch(run_controller_flag){
		case 0:
			break;
		case 1:
			error = reference - y;
			if (abs(error) > 10)
			{
				integral = integral + error*dt;
			}
			derivative = (error - prev_err)/dt;
			u = Kp*error + Ki*integral + Kd*derivative;
			prev_err = error;
			run_controller_flag = 0;
			break;
	}
	
	return (int)u/100;
}




void MOTOR_init( void )
{
	TWI_Master_Initialise();
	// Set MJ1 pins as output
	// PH4 = EN, PH1 = DIR
	DDRH |= (1 << PH4) | (1 << PH1);
	
	// Enable motor and select direction in MJ1
	PORTH |= (1 << PH4) | (1 << PH1);
	
	// Setup encoder: set MJ2 pins as input
	DDRK = 0x00;
	
	// Set some MJ1 pins to output, PH3 = SEL, PH5 = !OE, PH6 = RST
	DDRH |= (1 << PH3) | (1 << PH5)| (1 << PH6);
	
	//MOTOR_find_limits();
	//CONTROLLER_init_timer();

	
}

void MOTOR_find_limits( void )
{
	// *** Calibrate ***
	MOTOR_set_dir(MOTOR_right);
	max_velocity = init_velocity;
	MOTOR_set_velocity(init_velocity);
	_delay_ms(2000);
	MOTOR_set_velocity(0);
	
	// Reset encoder
	PORTH &= ~(1 << PH6);
	_delay_us(20);
	PORTH |= (1 << PH6);
	
	// Find max encoder value
	MOTOR_set_dir(MOTOR_left);
	MOTOR_set_velocity(init_velocity);
	_delay_ms(2000);
	MOTOR_set_velocity(0);
	MOTOR_max_encoder_value = MOTOR_read_encoder();
}

void MOTOR_set_dir(MOTOR_Direction dir)
{
	if (dir == MOTOR_right){
		// Right
		PORTH |= (1 << PH1);
	}
	else if (dir == MOTOR_left){
		// Left
		PORTH &= ~(1 << PH1);
	}
}

void MOTOR_set_max_velocity(int speed)
{
	switch (speed){
		case 1:
			max_velocity = 0x60;
			break;
		case 2:
			max_velocity = 0x80;
			break;
		default:
			max_velocity = 0x60;
		break;
	}
}

void MOTOR_set_velocity(uint8_t vel)
{
	uint8_t velocity[1];
	if (vel < max_velocity)
	{ 
		velocity[0] = vel; 
	}
	else
	{ 
		velocity[0] = max_velocity; 
	}
	TWI_send_address_and_data(velocity, 1);
	
	if(!TWI_Transceiver_Busy() && !TWI_statusReg.lastTransOK)
	{
		TWI_act_on_failure_in_last_transmission( TWI_Get_State_Info() );
	}
}

int16_t MOTOR_read_encoder( void )
{
	// Set !OE low
	PORTH &= ~(1<<PH5);
	
	// Set SEL low to get high byte(0)
	PORTH &= ~(1<<PH3);
	
	_delay_us(20); // TODO: is this necessary?!
	// Read MSB
	uint8_t encoder_value_MSB;
	encoder_value_MSB = PINK;
	
	// Set SEL high to get low byte(1)
	PORTH |= (1<<PH3);
	_delay_us(20);
	
	// Read LSB
	uint8_t encoder_value_LSB;
	encoder_value_LSB = PINK;

	// Set !OE high
	PORTH |= (1<<PH5);
	
	// Process received data
	int16_t total_encoder_value = encoder_value_MSB << 8 | encoder_value_LSB;
	
	return total_encoder_value;
}

int MOTOR_read_scaled_encoder( void )
{
	// Scaled between 0 and 255
	float encoder_value = (float)MOTOR_read_encoder()/MOTOR_max_encoder_value * 0xFF;
	return (int)encoder_value;
}


ISR(TIMER3_COMPA_vect)
{
	// reset interrupt
	TCNT3 = 0;

	// PID
	static int prev_error, integral, u;
	int y, error, derivative, dt;	

	y = MOTOR_read_scaled_encoder();
	error = g_reference - y;
	if (abs(error) > 10)
	{
		integral += error*dt;
	}
	derivative = (error - prev_error)/dt;
	u = Kp*error;// + Ki*integral + Kd*derivative;
	prev_error = error;

	MOTOR_set_dir(u > 0);
	MOTOR_set_velocity((uint8_t)u);
}
