#include "oled2.h"
#include "drawings.h"
#include "8x8font.h"

//***************************************************************
//	OLED cmd and data adresses and vars init					*
//***************************************************************
volatile char *OLED_cmd = (char *) 0x1000;
volatile char *OLED_data = (char *) 0x1200;
uint8_t page, col;


//***************************************************************
//	OLED init													*
//***************************************************************
void oled_init()
{
	//Setup the OLED display
	
	//display off
	*OLED_cmd = 0xAE;
	//segment remap
	*OLED_cmd = 0xA1;
	
	//common pads hardware: alternative
	*OLED_cmd = 0xDA;
	*OLED_cmd = 0x12;
	
	//common output scan direction:com63~com0
	*OLED_cmd = 0xC8;

	//multiplex ration mode:63
	*OLED_cmd = 0xA8;
	*OLED_cmd = 0x3F;

	//display divide ratio/osc. freq. mode
	*OLED_cmd = 0xD5;
	*OLED_cmd = 0x80;

	//contrast control
	*OLED_cmd = 0x81;
	*OLED_cmd = 0x50;

	//set pre-charge period
	*OLED_cmd = 0xD9;
	*OLED_cmd = 0x21;

	//set Memory Addressing Mode
	*OLED_cmd = 0x20;
	*OLED_cmd = 0x02;

	//VCOM deselect level mode
	*OLED_cmd = 0xDB;
	*OLED_cmd = 0x30;

	//master configuration
	*OLED_cmd = 0xAD;
	*OLED_cmd = 0x00;

	//out follows RAM content
	*OLED_cmd = 0xA4;

	//set normal display
	*OLED_cmd = 0xA6;
	//display on
	*OLED_cmd = 0xAF;
	
	//Set page start address
	*OLED_cmd = 0xB0;
	//Set lower column start address
	*OLED_cmd = 0x00;
	//Set higher column start address
	*OLED_cmd = 0x10;
	
	OLED_clear();
	OLED_home();
}


//***************************************************************
//	OLED functions												*
//***************************************************************
void cmd_OLED(char cmd)
{
	*OLED_cmd = cmd;
}

void data_OLED(char data)
{
	*OLED_data = data;
}

// Main function for printing buffer to OLED
void OLED_print_buffer(char** buffer){
	OLED_home();
	for (int i = 0; i < 8; i++){
		OLED_goto_line(i);
		for(int j = 0; j < 16; j++){
			OLED_print_char(buffer[i][j]);
		}
	}
}

int OLED_print_char(char c) {
	//Write the complete character
	for (int i = 0; i < FONTWIDTH; i++) {
		*OLED_data = pgm_read_byte(&myfont[c-' '][i]);
	}
	
	return 0;
}

int OLED_print(char *data) {
	int i = 0;
	
	//Write the complete string
	while(data[i] != '\0'){
		OLED_print_char(data[i]);
		i++;
	}
	
	return 0;
}

int OLED_home(void) {
	//Reset the page and column
	page = 0;
	col = 0;
	
	//Set the cursor to the start of the screen
	*OLED_cmd = 0x21;
	*OLED_cmd = 0x00;
	*OLED_cmd = 0x7f;
	
	*OLED_cmd = 0x22;
	*OLED_cmd = 0x00;
	*OLED_cmd = 0x7;
	
	return 0;
}

void OLED_clear_line(uint8_t line) {
	OLED_goto_line(line);
	
	//Clear the page
	for (int i = 0; i < 128; i++) {
		*OLED_data=0x00;
	}
	
	OLED_home();
	
}


void OLED_clear(void) {
	//Clear every page
	for (int k = 0; k < 8; k++) {
		OLED_clear_line(k);
	}
}


void OLED_goto_line(uint8_t line) {
	OLED_home();
	
	//Check if it's a valid page
	if (line < 8) {
		//Save the page we are moving to
		page = line;
		//Set lower column address
		*OLED_cmd = 0x00;
		//Set higher column address
		*OLED_cmd = 0x10;
		//Set page address
		*OLED_cmd = 0xB0 | line;
	}
}

void OLED_ARROW(){
	//OLED_goto_line(line);
	for(int i = 0; i < 12; i++){
		data_OLED(pgm_read_byte(&arrow[i]));
	}
}