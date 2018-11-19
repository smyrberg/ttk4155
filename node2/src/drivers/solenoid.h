/*!
 * @file solenoid.h
 * @brief Functions for controlling the solenoid.
 */
#ifndef SOLENOID_H_
#define SOLENOID_H_

/*!
 * Initializes the solenoid.
 */
void SOLENOID_init();

/*!
 * Shoot the solenoid
 * Blocks until the solenoid is back in normal position.
 */
void SOLENOID_shoot();

#endif /* SOLENOID_H_ */