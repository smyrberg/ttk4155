#include "menu.h"
#include "game.h"

#include "drivers/oled.h"
#include "drivers/joystick.h"

#include "common/uart.h"

#include <avr/delay.h>

#define MENU_ITEMS 4
#define NUM_SCORES 5



unsigned short cursor_line = 1;
game_score_t highscores[NUM_SCORES];

void MENU_init()
{
	game_score_t s = {.valid = 0};
	for (int i = 0; i < NUM_SCORES; i++)
	{
		highscores[i] = s;	
	}
}

void print_cursor()
{
	OLED_pos(cursor_line, 0);
	OLED_print_arrow();
}

void print_main_menu()
{
	OLED_reset();
	OLED_home();
	
	OLED_pos(0, ARROW_WIDTH);
	OLED_printf("=== SUPER GAME ===");
	
	OLED_pos(1, ARROW_WIDTH);
	OLED_printf("1. Start game");
	
	OLED_pos(2, ARROW_WIDTH);
	OLED_printf("2. View highscores");
	
	OLED_pos(3, ARROW_WIDTH);
	OLED_printf("3. Delete highscores");
	
	
	OLED_pos(4, ARROW_WIDTH);
	OLED_printf("4. Quit");
	
	print_cursor();
}

typedef enum cursor_direction {cursor_up_down, cursor_left_right} cursor_direction_t;

static int get_joystick_input(cursor_direction_t cursor_axis)
{
	JOY_position_t pos = JOY_get_position();
	int value = cursor_axis == cursor_up_down ? pos.y : pos.x;
	if (value > 240)
	{
		return 1;
	} 
	else if (value < 15)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

void MENU_cursor_up_on_screen()
{
	if (cursor_line > 1)
	{
		OLED_pos(cursor_line, 0);
		OLED_clear_arrow();
		cursor_line--;
		OLED_pos(cursor_line, 0);
		OLED_print_arrow();
	}
}

void MENU_cursor_down_on_screen()
{
	if (cursor_line < MENU_ITEMS)
	{
		OLED_pos(cursor_line, 0);
		OLED_clear_arrow();
		cursor_line++;
		OLED_pos(cursor_line, 0);
		OLED_print_arrow();
	}
}

static void move_cursor_y(int dir)
{
	if (dir == 0)
	{
		return;
	}
	
	if (dir == -1)
	{
		MENU_cursor_down_on_screen();	
	}
	else if (dir == 1)
	{
		MENU_cursor_up_on_screen();
	}
	
	while (get_joystick_input(cursor_up_down) != 0 );
	
}

static void print_score(int pos, game_score_t s)
{
	if (s.valid)
	{
		OLED_printf("%d. Time: %02d Fail: %02d",pos, s.time, s.failures);
	}
	else {
		OLED_printf("%d. BLANK", pos);
	}
}

void MENU_view_highscores()
{
	OLED_reset();
	OLED_pos(0, ARROW_WIDTH);
	OLED_printf("=== HIGHSCORES ===");
	for (int i = 1;i <= NUM_SCORES; i++)
	{
		OLED_pos(i, ARROW_WIDTH);
		print_score(i, highscores[i-1]);
	}
	
	OLED_pos(7, ARROW_WIDTH);
	OLED_printf("push left to return");
	
	for(;;)
	{
		uint8_t left = get_joystick_input(cursor_left_right) == -1 ? 1 : 0;
		if (left)
		{
			return;
		}
	}
}


void delete_highscores()
{
	OLED_reset();
	OLED_printf("deleting highscores...");
	MENU_init();
	_delay_ms(1000);
	return;
}

void insert_score(game_score_t score, game_score_t *table)
{
	for (int i = 0; i < NUM_SCORES; i++)
	{
		if (score.failures < highscores[i].failures || !highscores[i].valid)
		{
			for (int j = NUM_SCORES-1; j > i; j--)
			{
				highscores[j] = highscores[j-1];
			}
			highscores[i] = score;
			return;
		}
	}	
}


uint8_t MENU_main()
{
	print_main_menu();
	
	int dir;
	game_score_t score;
	for(;;)
	{
		move_cursor_y(get_joystick_input(cursor_up_down));
		
		uint8_t right = get_joystick_input(cursor_left_right) == 1 ? 1 : 0;
		if (right)
		{
			switch(cursor_line)
			{
				case 1:
					// start game
					score = GAME_play();
					print_main_menu();
					insert_score(score, highscores);
					break;
				case 2:
					// view highscores
					MENU_view_highscores();
					print_main_menu();
					break;
				case 3:
					delete_highscores();
					print_main_menu();
					break;
				case 4:
					OLED_reset();
					OLED_printf("see ya");
					// quit
					return;
			}
		}
		
	}
	
	return 0;
}