#include "motor.h"
#include "twi_handler.h"
#include "TWI_Master.h"
#include "common/uart.h"
#include <avr/delay.h>
#include <math.h>


// file globals
static motor_mode_t g_mode = MOTOR_mode_no_ctrl;
static int16_t g_max_encoder_value;
static uint8_t g_reference_position = 127;

// internal functions
static void start_controller_timer();
static int read_encoder_scaled();
static int16_t read_encoder_raw();
static void set_speed(uint8_t speed);
static void set_direction(uint8_t direction);


void MOTOR_init()
{
	TWI_Master_Initialise();
	// set MJ1 pins as output
	// PH4 = EN, PH1 = DIR
	DDRH |= (1 << PH4) | (1 << PH1);
	
	// enable motor and select direction in MJ1
	PORTH |= (1 << PH4) | (1 << PH1);
	
	// setup encoder: set MJ2 pins as input
	DDRK = 0x00;
	
	// set some MJ1 pins to output, PH3 = SEL, PH5 = !OE, PH6 = RST
	DDRH |= (1 << PH3) | (1 << PH5)| (1 << PH6);
	
	//start_controller_timer();
	
	printf("[MOTOR] INFO: initialization done\r\n");
	
}

void MOTOR_find_limits()
{
	// drive to end
	MOTOR_set_direction(MOTOR_right);
	MOTOR_set_speed(96);
	_delay_ms(2000);
	MOTOR_set_speed(0);
	
	// reset encoder
	PORTH &= ~(1 << PH6);
	_delay_us(20);
	PORTH |= (1 << PH6);
	
	// find max encoder value
	MOTOR_set_direction(MOTOR_left);
	MOTOR_set_speed(96);
	_delay_ms(2000);
	MOTOR_set_speed(0);
	g_max_encoder_value = read_encoder_raw();
	printf("[MOTOR] INFO: max encoder value found: %d\r\n", g_max_encoder_value);
}

void MOTOR_set_mode(motor_mode_t mode)
{
	char *old_mode = g_mode == MOTOR_mode_ctrl ? "MOTOR_mode_pid" : "MOTOR_mode_no_ctrl";
	char *new_mode = mode == MOTOR_mode_ctrl ? "MOTOR_mode_pid" : "MOTOR_mode_no_ctrl";	
	printf("[MOTOR] INFO: mode set (%s -> %s)\r\n", old_mode, new_mode);
	g_mode = mode;
}

motor_mode_t MOTOR_get_mode()
{
	return g_mode;
}

void MOTOR_set_position(uint8_t position)
{
	if (g_mode != MOTOR_mode_ctrl)
	{
		printf("[MOTOR] WARNING: setting position outside of PID mode\r\n");
	}
	g_reference_position = position;
}

void MOTOR_set_speed(uint8_t speed)
{
	if (g_mode == MOTOR_mode_ctrl)
	{
		printf("[MOTOR] WARNING: setting speed directly in PID mode\r\n");
	}
	set_speed(speed);
}

void set_speed(uint8_t speed)
{	
	speed = (speed > 150) ? 150 : speed;
	
	TWI_send_address_and_data(&speed, 1);
	
	if(!TWI_Transceiver_Busy() && !TWI_statusReg.lastTransOK)
	{
		TWI_act_on_failure_in_last_transmission(TWI_Get_State_Info());
	}
}

void MOTOR_set_direction(motor_direction_t direction)
{
	if (g_mode == MOTOR_mode_ctrl)
	{
		printf("[MOTOR] WARNING: setting direction directly in PID mode\r\n");
	}
	set_direction(direction);
}


void set_direction(motor_direction_t direction)
{
	switch(direction)
	{
		case MOTOR_left:
			PORTH &= ~(1 << PH1);
			break;
		case MOTOR_right:
			PORTH |= (1 << PH1);
			break;
	}
}

static int16_t read_encoder_raw()
{
	// set !OE low
	PORTH &= ~(1<<PH5);
	
	// set SEL low to get high byte(0)
	PORTH &= ~(1<<PH3);
	
	_delay_us(20); // TODO: is this necessary?!
	// read MSB
	uint8_t encoder_value_MSB;
	encoder_value_MSB = PINK;
	
	// set SEL high to get low byte(1)
	PORTH |= (1<<PH3);
	_delay_us(20);
	
	// read LSB
	uint8_t encoder_value_LSB;
	encoder_value_LSB = PINK;

	// set !OE high
	PORTH |= (1<<PH5);
	
	// process received data
	int16_t total_encoder_value = encoder_value_MSB << 8 | encoder_value_LSB;
	
	return total_encoder_value;
}

#define Kp 110
#define Ki 10
#define Kd 100
volatile int run_controller_flag = 0;

void MOTOR_controller_update()
{
	if (g_mode != MOTOR_mode_ctrl)
	{
		printf("[MOTOR] WARNING: unneccesary call to MOTOR_pid_update() outside MOTOR_mode_pid\r\n");
		return;
	}
	
	static int integral;
	static int u;
	static int prev_err;
	int error;
	int derivative;
	int dt = 1;		//0.1*10
	uint8_t y = read_encoder_scaled();
		
	switch(run_controller_flag)
	{
		case 0:
			break;
		case 1:
			error = g_reference_position - y;
			if (abs(error) > 10){
				integral = integral + error*dt;
			}
			derivative = (error - prev_err)/dt;
			u = Kp*error + Ki*integral + Kd*derivative;
			prev_err = error;
			run_controller_flag = 0;
			break;
	}
		
	MOTOR_set_direction(u < 0);
	MOTOR_set_speed(u);
}

static int read_encoder_scaled()
{
	// Scaled between 0 and 255
	float encoder_value = (float)read_encoder_raw()/g_max_encoder_value * 255;
	return (int)encoder_value;
}


static void start_controller_timer()
{
	// OC3A disconnected. Normal port operation, Normal mode
	
	// set prescaler to 1/64
	TCCR3B |= (1 << CS31) | (1 << CS30);
	
	// set the desired output compare match that will generate a timer interrupt
	// choose dt = 0.1 -> OCR3A = 12500
	OCR3A = 0x30D4;
	
	// enable output compare interrupt 3A
	TIMSK3 |= (1 << OCIE3A);
}


ISR(TIMER3_COMPA_vect)
{
	TCNT3 = 0;
	run_controller_flag = 1;
}

