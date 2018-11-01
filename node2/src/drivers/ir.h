#ifndef IR_H_
#define IR_H_

#include <avr/io.h>

void IR_init();
uint16_t IR_read();
uint8_t IR_beam_broken();

#endif /* IR_H_ */