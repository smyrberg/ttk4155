#include "oled.h"
#include "adc.h"
#include <avr/io.h>
#include <stdio.h>
#include "font_normal.h"

#define OLED_COMMAND_ADDRESS                0x1000
#define OLED_DATA_ADDRESS                   0x1200

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

volatile uint8_t *oled_cmd = (uint8_t *) OLED_COMMAND_ADDRESS;	// Start address for the OLED command
volatile uint8_t *oled_data = (uint8_t *) OLED_DATA_ADDRESS;	// Start address for the OLED data
uint8_t page, col;

void OLED_init( void )
{

	*oled_cmd = 0xAE;	// display off
	*oled_cmd = 0xA1;	// segment remap
	
	//Common pads hardware: alternative
	*oled_cmd = 0xDA;
	*oled_cmd = 0x12;
	
	*oled_cmd = 0xC8;	//Common output scan direction:com63~com0
	
	//Multiplex ration mode:63
	*oled_cmd = 0xA8;
	*oled_cmd = 0x3F;

	//Display divide ratio/osc. freq. mode
	*oled_cmd = 0xD5;
	*oled_cmd = 0x80;

	//Contrast control
	*oled_cmd = 0x81;
	*oled_cmd = 0x50;

	//Set pre-charge period
	*oled_cmd = 0xD9;
	*oled_cmd = 0x21;

	//Set Memory Addressing Mode
	*oled_cmd = 0x20;
	*oled_cmd = 0x00;	// horizontal addresssing mode

	//VCOM deselect level mode
	*oled_cmd = 0xDB;
	*oled_cmd = 0x30;

	//Master configuration
	*oled_cmd = 0xAD;
	*oled_cmd = 0x00;

	*oled_cmd = 0xA4;	//Out follows RAM content
	*oled_cmd = 0xA6;	//Set normal display
	*oled_cmd = 0xAF;	//Display on

	// clear screen
	for(int i = 0; i < 128*8; i++){
		OLED_write_data(0x0);
	}

	OLED_home();

}

void OLED_write_command(uint8_t command){
	*oled_cmd = command;
}

void OLED_write_data( uint8_t data ){
	*oled_data = data;
}


void OLED_goto_column(uint8_t column){
	*oled_cmd = 0x21;
	*oled_cmd = column;
	*oled_cmd = 127;
}

void OLED_goto_row(uint8_t row){
	*oled_cmd = 0x22;
	*oled_cmd = row;
	*oled_cmd = 7;
}

void OLED_pos(uint8_t row, uint8_t column)
{
	OLED_goto_row(row);
	OLED_goto_column(column);
}

void OLED_home()
{
	page = 0;
	col = 0;
	
	*oled_cmd = 0x22;
	*oled_cmd = page;
	*oled_cmd = 0x07;	
	
	*oled_cmd = 0x21;
	*oled_cmd = col;
	*oled_cmd = 0x7f;

//OLED_goto_column(0);
//OLED_goto_row(0);
}


void OLED_clear_line( uint8_t line )
{
	// go to start position: first column in this line
	OLED_pos(line, 0);
	
	// go through all columns
	for (uint8_t col = 0; col < NUM_COLUMNS; col++){
		// write 8 bits to each column
		OLED_write_data(0x00);
	}
}

void OLED_reset( void )
{
	// clear all lines
	for (uint8_t line = 0; line < NUM_LINES; line++){
		OLED_clear_line(line);
	}
	
	// go home
	OLED_home();
}


void OLED_set_contrast( uint8_t level )
{
	OLED_write_command(SET_CONTRAST_CONTROL);
	OLED_write_command(level);
}

void OLED_print_char( char *character, uint8_t line)
{	
	//printf("Character: %c \n", *character);
	if ((*character) == '\n'){
		//printf("newline\n");
		OLED_pos(line + 1, 0);
	}
	else{
		int i;
		for (i = 0; i < 5; i++){
			OLED_write_data(pgm_read_byte(&font5[*character - ' '][i]));
		}
	}
}

void OLED_print(char *c, uint8_t line)
{
	int i = 0;
	while(c[i] != '\0'){
		//printf("From OLED_print %c \n", c[i]);
		OLED_print_char(&c[i], line);
		i++;
	}
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


void OLED_smiley()
{
	OLED_pos(2,32);
	for (int col = 0; col < 10; col++){
		OLED_write_data(0xFF);
	}
	OLED_pos(2,85);
	for (int col = 0; col < 10; col++){
		OLED_write_data(0xFF);
	}
	
	OLED_pos(4, 32);
	for (int col = 0; col < 63; col++){
		OLED_write_data(0xF0);
	}
	
	OLED_pos(5, 42);
	for (int col = 0; col < 43; col++){
		OLED_write_data(0xFF);
	}
	


	OLED_pos(6, 57);
	for (int col = 0; col < 13; col++){
		OLED_write_data(0xFF);
	}
}