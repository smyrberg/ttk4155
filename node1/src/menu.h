#ifndef MENU_H_
#define MENU_H_

#include <stdio.h>


void MENU_init();
uint8_t MENU_main();

void MENU_print_main();
void MENU_print_highscore();
void MENU_clear_highscore();
void MENU_add_score();

void MENU_cursor_up_on_screen();
void MENU_cursor_down_on_screen();

#endif /* MENU_H_ */