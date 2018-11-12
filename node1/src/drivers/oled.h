
#ifndef OLED_H_
#define OLED_H_

#define NUM_LINES							8
#define NUM_COLUMNS							128

#define SEGMENT_REMAP						0xA1
#define ENTIRE_DISPLAY_ON					0xA4
#define SET_NORMAL_DISPLAY					0xA6
#define SET_INVERSE_DISPLAY					0xA7
#define SET_MULTIPLEX_RATIO					0xA8

#define SELECT_IREF							0xAD
#define DISPLAY_OFF							0xAE
#define DISPLAY_ON							0xAF

#define SET_MEMORY_ADDRESSING_MODE			0x20
#define HORIZONTAL_ADDR_MODE				0x00
#define VERTICAL_ADDR_MODE					0x01
#define PAGE_ADDR_MODE						0x02

#define SET_COLUMN_ADDRESS					0x21	//0-127 or 0x00-0x7F
#define SET_PAGE_ADDRESS					0x22	//0-7 or 0x0 to 0x7

#define SET_CONTRAST_CONTROL				0x81	//256 contrast steps from 0x00 to 0xFF
/* Contrast levels */
#define LOW_CONTRAST						0x10
#define MEDIUM_CONTRAST						0x50
#define HIGH_CONTRAST						0xF0

#define SET_COM_OUT_SCAN_DIR				0xC8	// com63 to com0
#define SET_DISPLAY_CLK_DIV_RATIO_OSC_FREQ	0xD5
#define SET_PRECHARGE_PERIOD				0xD9
#define SET_COM_PIN_HW_CONFIG				0xDA
#define SET_VCOM_DESELECT_LVL				0xDB

#define ARROW_WIDTH 12


#include <avr/io.h>

void OLED_init( void );
void OLED_write_command( uint8_t command );
void OLED_write_data( uint8_t data );
void OLED_goto_column( uint8_t column );
void OLED_goto_line( uint8_t l );
void OLED_pos(uint8_t row, uint8_t column);
void OLED_home( void );
void OLED_clear_line( uint8_t line );
void OLED_reset( void );
void OLED_set_contrast( uint8_t level );
void OLED_invert_screen();
void OLED_print_char( char character);
void OLED_print_arrow();
void OLED_clear_arrow();
void OLED_printf(const char* fmt, ...);
void OLED_smiley();

//void OLED_SRAM_flush();
//void OLED_SRAM_clear_line(uint8_t line);
//void OLED_SRAM_clear_screen();
//void OLED_SRAM_write_char(char c, uint8_t line, uint16_t col);
//void OLED_SRAM_write(const char* fmt, uint8_t line, uint16_t col, ...);


#endif /* OLED_H_ */