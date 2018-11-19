/*!
 * @file oled.h
 * @brief Driver printing to the OLED screen.
 */
#ifndef OLED_H_
#define OLED_H_

#include <avr/io.h>
#define ARROW_WIDTH 12 /*!< Pixel width of arrows that can be printed*/

/*!
 * Initializes the joystick driver.
 */
void OLED_init();


/*!
 * Move the caret to the specified column.
 */
void OLED_goto_column(uint8_t column);

/*!
 * Move the caret to the specified row.
 */
void OLED_goto_line(uint8_t l);

/*!
 * Move the caret to the specified row and column.
 */
void OLED_pos(uint8_t row, uint8_t column);

/*!
 * Move the caret to row 0 and column 0.
 */
void OLED_home();

/*!
 * Clear entire line.
 */
void OLED_clear_line(uint8_t line);

/*!
 * Clear the entire screen.
 */
void OLED_reset();

/*!
 * Set the contrast of the screen.
 */
void OLED_set_contrast(uint8_t level);

/*!
 * Invert screen colors.
 */
void OLED_invert_screen();

/*!
 * Print single ASCII character.
 */
void OLED_print_char(char character);

/*!
 * Print an arrow pointing right.
 */
void OLED_print_arrow();

/*!
 * Print an arrow pointing left.
 */
void OLED_print_back_arrow();

/*!
 * Clear an arrow with of screen space. Useful for removing exactly one arrow at the current location.
 */
void OLED_clear_arrow();

/*!
 * Print formatted strings to the screen.
 */
void OLED_printf(const char* fmt, ...);

#endif /* OLED_H_ */