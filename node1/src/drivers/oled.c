#include "oled.h"
#include "font_normal.h"
#include "common/uart.h"

#define OLED_COMMAND_ADDRESS	0x1000
#define OLED_DATA_ADDRESS		0x1200
#define ADC_DATA_ADDRESS		0x1400
#define SRAM_DATA_ADDRESS		0x1800

#include <avr/io.h>
#include <stdio.h>

// Start address for the OLED command
volatile uint8_t *oled_cmd = (uint8_t *) OLED_COMMAND_ADDRESS;
// Start address for the OLED data
volatile uint8_t *oled_data = (uint8_t *) OLED_DATA_ADDRESS;

static FILE oled_stdout = FDEV_SETUP_STREAM(OLED_print_char, NULL, _FDEV_SETUP_WRITE);
static uint8_t line = 0;

void OLED_init( void )
{
	
	OLED_write_command(DISPLAY_OFF);
	OLED_write_command(SEGMENT_REMAP);

	OLED_write_command(SET_COM_PIN_HW_CONFIG);        // Common  pads  hardware:  alternative
	OLED_write_command(0x12);						  // 0x12 = bx00010010, meaning A[5] = 0, A[4] = 1
	OLED_write_command(SET_COM_OUT_SCAN_DIR);		  // Common output scan direction:com63~com0

	OLED_write_command(SET_MULTIPLEX_RATIO);
	OLED_write_command(0x3f);						  // Multiplex  ratio  mode: 63

	OLED_write_command(SET_DISPLAY_CLK_DIV_RATIO_OSC_FREQ);        // Display divide ratio/osc. freq. mode
	OLED_write_command(0x80);

	OLED_write_command(SET_CONTRAST_CONTROL);
	OLED_write_command(0x50);

	OLED_write_command(SET_PRECHARGE_PERIOD);         // Set  pre-charge  period
	OLED_write_command(0x21);						  // Period

	OLED_write_command(SET_MEMORY_ADDRESSING_MODE);
	OLED_write_command(HORIZONTAL_ADDR_MODE);

	OLED_write_command(SET_VCOM_DESELECT_LVL);
	OLED_write_command(0x30);

	OLED_write_command(SELECT_IREF);				  // Master configuration cmd - select external or internal Iref
	OLED_write_command(0x00);						  // 0x00: external, 0x01: internal

	OLED_write_command(ENTIRE_DISPLAY_ON);			  // Out  follows  RAM  content

	OLED_write_command(SET_NORMAL_DISPLAY);
	OLED_write_command(DISPLAY_ON);
	
	OLED_reset();
	
	// Set display offset
	OLED_write_command(0xD3);
	// OLED_write_command(0x20);
	OLED_write_command(0x00);
}

void OLED_write_command( uint8_t command )
{
	*oled_cmd = command;
}

void OLED_write_data( uint8_t data )
{
	*oled_data = data;
}


void OLED_goto_column( uint8_t column )
{
	if (column < NUM_COLUMNS){
		OLED_write_command(SET_COLUMN_ADDRESS);
		OLED_write_command(column);
		OLED_write_command(0x7F);
	}
}

void OLED_goto_line( uint8_t l )
{
	if (l < NUM_LINES){
		OLED_write_command(SET_PAGE_ADDRESS);
		line = l;
		OLED_write_command(line);
		OLED_write_command(7);
	}
}

void OLED_pos(uint8_t row, uint8_t column)
{
	OLED_goto_line(row);
	OLED_goto_column(column);
}

void OLED_home( void )
{
	OLED_pos(0, 0);
}


void OLED_clear_line( uint8_t line )
{
	OLED_pos(line, 0);
	
	for (uint8_t col = 0; col < NUM_COLUMNS; col++){
		// Write 8 bits to each column
		OLED_write_data(0x00);
	}
}

void OLED_reset( void )
{
	for (uint8_t line = 0; line < NUM_LINES; line++){
		OLED_clear_line(line);
	}
	OLED_home();
}


void OLED_set_contrast( uint8_t level )
{
	OLED_write_command(SET_CONTRAST_CONTROL);
	OLED_write_command(level);
}

void OLED_invert_screen()
{
	static int inverted;
	if (!inverted){
		OLED_write_command(SET_INVERSE_DISPLAY);
		inverted = 1;
		}else{
		OLED_write_command(SET_NORMAL_DISPLAY);
		inverted = 0;
	}
}

void OLED_print_char( char character )
{
	if ((character) == '\n'){
		OLED_pos((line + 1), 0);
	}
	else{
		for (int i = 0; i < 5; i++){
			OLED_write_data(pgm_read_byte(&font5[character - ' '][i]));
		}
	}
}

void OLED_printf(const char* fmt, ...){
	va_list args;
	va_start(args, fmt);
	vfprintf(&oled_stdout, fmt, args);
	va_end(args);
}

void OLED_print_arrow( void )
{
	OLED_write_data(0b00011000);
	OLED_write_data(0b00011000);
	OLED_write_data(0b00011000);
	OLED_write_data(0b00011000);
	OLED_write_data(0b00011000);
	OLED_write_data(0b11111111);
	OLED_write_data(0b01111110);
	OLED_write_data(0b00111100);
	OLED_write_data(0b00011000);
	OLED_write_data(0b00000000);

}

void OLED_print_back_arrow()
{
	OLED_write_data(0b00000000);
	OLED_write_data(0b00000000);
	OLED_write_data(0b00000000);
	OLED_write_data(0b00011000);
	OLED_write_data(0b00100100);
	OLED_write_data(0b01000010);
	OLED_write_data(0b10000001);
	OLED_write_data(0b00000000);
	OLED_write_data(0b00000000);
	OLED_write_data(0b00000000);
	
	
}

void OLED_clear_arrow()
{
	for (int i = 0; i < 12; i++)
	{
		OLED_write_data(0b00000000);
	}
}