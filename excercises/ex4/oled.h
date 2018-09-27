#ifndef OLED_H_
#define OLED_H_

#include <avr/io.h>


void OLED_init( void );
void OLED_write_command( uint8_t command );
void OLED_write_data( uint8_t data );
void OLED_goto_column( uint8_t column );
void OLED_goto_line( uint8_t line );
void OLED_pos(uint8_t row, uint8_t column);
void OLED_home();
void OLED_clear_line( uint8_t line );
void OLED_reset( void );
void OLED_set_contrast( uint8_t level );
void OLED_print_char( char *character, uint8_t line);
void OLED_print(char *c, uint8_t line);
void OLED_print_arrow( void );

void OLED_smiley();

#endif /* OLED_H_ */