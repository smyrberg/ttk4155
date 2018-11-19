#include "menu.h"
#include "game.h"
#include "scores.h"
#include "menu_items.h"
#include "common/uart.h"
#include "drivers/joystick.h"
#include "drivers/oled.h"
#include <avr/delay.h>

#define LOOP_DELAY_MS 40

// internal declarations
static menu_direction_t get_joystick_direction();
static menu_t* get_next_menu_from_user(menu_t *menu);
static void print_menu(menu_t *menu);

void MENU_init()
{
	SCORES_delete();
	printf("[MENU] INFO: initialization done\r\n");
}

uint8_t MENU_main()
{
	menu_t* current_menu = &m_main;
	menu_t* next_menu;
	while(1)
	{
		
		print_menu(current_menu);
		next_menu = get_next_menu_from_user(current_menu);
		
		if (next_menu->function_ptr)
		{
			next_menu->function_ptr();
		}
		else if (next_menu->game_ptr)
		{
			SCORES_add(next_menu->game_ptr());
		}
		else 
		{
			printf("[MENU] INFO: new menu selected: %s\r\n", next_menu->name);
			current_menu = next_menu;
		}
	}
}

static menu_t* get_next_menu_from_user(menu_t *menu)
{
	uint8_t line = 1;
	
	while(1)
	{
		OLED_pos(line, 0);
		OLED_print_arrow();
		
		switch(JOY_get_4axis_direction())
		{
			case menu_up:
				if (line > 1)
				{
					OLED_pos(line, 0);
					OLED_clear_arrow();
					line--;
				}
				break;
			case menu_down:
				if (line < menu->child_count)
				{
					OLED_pos(line, 0);
					OLED_clear_arrow();
					line++;
				}
				break;
			case menu_right:
				return menu->children[line-1];
			case menu_left:
				return menu->parent;
		}
	}
}

static void print_menu(menu_t *menu)
{
	OLED_reset();
	OLED_pos(0, ARROW_WIDTH);
	OLED_printf("%s", menu->name);
	
	// print children
	for (int i = 0; i < menu->child_count; i++)
	{
		OLED_pos(i+1, ARROW_WIDTH);
		OLED_printf("%d. %s", i+1, menu->children[i]->name);
	}
	
	// print parent
	uint8_t is_root = menu->parent == menu;
	if (menu->parent != NULL && !is_root)
	{
		OLED_pos(7, 0);
		OLED_print_back_arrow();
		OLED_printf("%s", menu->parent->name);
	}
}
