/*!
 * @file ir.h
 * @brief Functions for detecting IR.
 */
#ifndef IR_H_
#define IR_H_

#include <stdbool.h>

/*!
 * Initializes the IR driver, and the ADC used for reading IR values.
 */
void IR_init();

/*!
 * Detects if the IR beam is broken.
 * @return True if IR beam is broken. Otherwise false.
 */
bool IR_beam_broken();

#endif /* IR_H_ */