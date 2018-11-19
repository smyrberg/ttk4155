/*!
 * @file servo.h
 * @brief Functions for controlling the servo.
 */
#ifndef SERVO_H_
#define SERVO_H_

#include <avr/io.h>

/*!
 * Initializes the servo.
 * The default position is ca. 127
 */
void SERVO_init();

/*!
 * Sets the position of the servo
 * @param[in] pos Position to go to (0-255).
 */
void SERVO_set_position(uint8_t pos);

#endif /* SERVO_H_ */