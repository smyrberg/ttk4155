/*!
 * @file motor.h
 * @brief Functions for controlling the motor.
 */
#ifndef MOTOR_H_
#define MOTOR_H_

#include <avr/io.h>

/*!
 * Modes available for the motor of the motor.
 */
typedef enum motor_mode_t {
	MOTOR_mode_ctrl = 0,
	MOTOR_mode_no_ctrl
} motor_mode_t;

/*!
 * Directions the motor may move in.
 */
typedef enum motor_direction_t {
	MOTOR_right = 0,
	MOTOR_left
} motor_direction_t;

/*!
 * Initializes the motor.
 * The mode is the motor is in is arbitrary.
 */
void MOTOR_init();

/*!
 * Finds the encoder limits by driving to each side of the game board.
 * Encoder limits are stored internally, and needed for PID control.
 */
void MOTOR_find_limits();

/*!
 * Sets the mode for the motor
 * @param[in] mode The mode to set the motor in.
 */
void MOTOR_set_mode(motor_mode_t mode);

/*!
 * Get the current mode for the motor
 * @return The mode the motor is in.
 */
motor_mode_t MOTOR_get_mode();

/*!
 * Sets the position to go to in PID mode.
 * @param[in] position Position to go to (0-255).
 */
void MOTOR_set_position(uint8_t position);

/*!
 * Sets the speed of the motor in no control mode.
 * @param[in] speed The speed to drive with. Clamped at 150.
 */
void MOTOR_set_speed(uint8_t speed);

/*!
 * Sets the direction of the motor in no control mode.
 * @param[in] direction Direction of motor.
 */
void MOTOR_set_direction(motor_direction_t direction);

/*!
 * Updates the speed and direction with respect to the reference position.
 * It will internally read the actual position from the motor encoder.
 */
void MOTOR_controller_update();

#endif /* MOTOR_H_ */