/*!
 * @file joystick.h
 * @brief Driver for reading joystick input and button presses.
 */
#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <avr/io.h>

/*!
 * @brief Enumeration defining discrete 4-axis directions, for use in a menu context.
 */
typedef enum {
	menu_up,
	menu_down,
	menu_left,
	menu_right,
} menu_direction_t;

/*!
 * @brief Structure holding the xy-position of the joystick.
 */
typedef struct JOY_pos {
	uint8_t x;		/*!< Horizontal axis*/
	uint8_t y;		/*!< Vertical axis*/
} JOY_position_t;

/*!
 * Initializes the joystick driver.
 */
void JOY_init();

/*!
 * Return a discrete 4axis direction.
 * Blocks until the user have given one direction and have returned the joystick to a neutral position.
 * @return 4axis direction provided by the user.
 */
menu_direction_t JOY_get_4axis_direction();

/*!
 * Poll whether the right button is pressed on the IO board.
 * Non-blocking.
 * @return 1 if button is pressed, otherwise 0.
 */
int JOY_right_button();

/*!
 * Poll the joystick position.
 * @return The xy position of the joystick.
 */
JOY_position_t JOY_get_position();



#endif /* JOYSTICK_H_ */