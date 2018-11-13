#include "menu.h"
#include "game.h"

#include "drivers/oled.h"
#include "drivers/joystick.h"

#include "common/uart.h"

#include <avr/delay.h>

#define MENU_ITEMS 4
#define NUM_SCORES 5

// must declare type first so it can be self-referential
typedef struct menu_t menu_t;
typedef struct menu_t
{
	char *name;
	menu_t *parent;
	menu_t *children[MENU_ITEMS];
	uint8_t child_count;
	void (*function_ptr)(void);
};

// internal function declarations
static void delete_highscores();
static void view_highscores();

// declare all menus (as they are referencing each other)
menu_t m_main, m_start_game, m_quit;
menu_t m_highscores, m_highscores_view, m_highscores_delete;
menu_t m_settings, m_settings_invert;

// menu items
menu_t m_main			   = {.name="Main Menu",		 .parent=&m_main, .children={&m_start_game, &m_highscores_view, &m_highscores_delete, &m_quit}, .child_count=4, .function_ptr=NULL};
menu_t m_start_game		   = {.name="Start Game",		 .parent=&m_main, .children=NULL, .child_count=0, .function_ptr=NULL};
menu_t m_quit			   = {.name="Quit",				 .parent=&m_main, .children=NULL, .child_count=0, .function_ptr=NULL};
menu_t m_highscores		   = {.name="Highscores",		 .parent=&m_main, .children={&m_highscores_view, &m_highscores_delete}, .child_count=2, .function_ptr=NULL};
menu_t m_highscores_view   = {.name="View Highscores",   .parent=&m_highscores, .children=NULL, .child_count=0, .function_ptr=&view_highscores};
menu_t m_highscores_delete = {.name="Delete Highscores", .parent=&m_highscores, .children=NULL, .child_count=0, .function_ptr=&delete_highscores};
menu_t m_settings		   = {.name="Settings",			 .parent=&m_main,  .children={&m_settings_invert}, .child_count=1, .function_ptr=NULL};
menu_t m_settings_invert   = {.name="Invert Screen",	 .parent=&m_settings, .children=NULL, .child_count=0, .function_ptr=&OLED_invert_screen};
	
void print_menu_items(menu_t *menu)
{	
	// print header
	OLED_pos(0, ARROW_WIDTH);
	OLED_printf("%s", menu->name);
	
	// print children
	if (menu->children)
	{
		int i = 0;
		while (menu->children[i] != NULL)
		{
			OLED_pos(i+1, ARROW_WIDTH);
			OLED_printf("%d. %s", i+1, menu->children[i]->name);
		}
	}
	
	// print parent
	uint8_t is_root = menu->parent == menu->parent;
	if (menu->parent != NULL && !is_root)
	{
		OLED_pos(MENU_ITEMS+2, 0);
		OLED_print_back_arrow();
		OLED_printf("%s", menu->parent->name);
	}
}


typedef enum {menu_up, menu_down, menu_left, menu_right, menu_nop} menu_direction_t;
static menu_direction_t get_joystick_direction()
{
	JOY_position_t pos = JOY_get_position();
	if (pos.y > 240) 
	{ 
		while(JOY_get_position().y > 240);
		return menu_down;  
	}
	if (pos.y <  15) 
	{ 
		while(JOY_get_position().y < 15);
		return menu_up;    
	}
	if (pos.x > 240) 
	{ 
		while(JOY_get_position().x > 240);
		return menu_right; 
	}
	if (pos.x < 15 ) 
	{ 
		while(JOY_get_position().x < 15);
		return menu_left;  
	}
}

static uint8_t move_cursor(menu_direction_t direction)
{
	static uint8_t line = 0;
	
	// clear current arrow
	OLED_pos(line+1, 0);
	OLED_clear_arrow();
	
	switch(direction)
	{
		case menu_up:
			line = line > 0 ? line-1 : 0;
			break;
		case menu_down:
			line = line < MENU_ITEMS ? line+1 : MENU_ITEMS;
			break;
		case menu_left:
		case menu_right:
		case menu_nop:
			break;
	}
	
	// draw new arrow
	OLED_pos(line+1, 0);
	OLED_print_arrow();
	return line;
}

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

static void view_highscores()
{
	OLED_reset();
	OLED_pos(0, ARROW_WIDTH);
	OLED_printf("Highscore List");
	for (int i = 1;i <= NUM_SCORES; i++)
	{
		OLED_pos(i, ARROW_WIDTH);
		print_score(i, highscores[i-1]);
	}
	
	OLED_pos(MENU_ITEMS+2, 0);
	OLED_print_back_arrow();
	
	while(get_joystick_direction() != menu_left);
}

static void delete_highscores()
{
	OLED_reset();
	OLED_printf("deleting highscores...");
	MENU_init();
	_delay_ms(1000);
	return;
}

static void insert_score(game_score_t score, game_score_t *table)
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
	menu_t *current_menu = &m_main;
	print_menu_items(current_menu);
	
	// get cursor position
	uint8_t cursor_pos = move_cursor(menu_nop);
	
	menu_direction_t menu_direction;
	for(;;)
	{
		menu_direction = get_joystick_direction();
		switch(menu_direction)
		{
			case menu_up:
			case menu_down:
				 cursor_pos = move_cursor(menu_direction);
				 break;
			case menu_right:
				// Check if there is a valid menu entry at cursor
				if (cursor_pos <= current_menu->child_count)
				{
					if(current_menu->children[cursor_pos]->function_ptr)
					{	// menu item has a function
						current_menu->children[cursor_pos]->function_ptr();
					}
					else
					{	// menu item don't have a function (implicitly a submenu)
						current_menu = current_menu->children[cursor_pos];
					}
					print_menu_items(current_menu);
				}
				break;
			case menu_left:
				current_menu = current_menu->parent;
				print_menu_items(current_menu);
				break;
		}
	}
}


#if 0 // OLD MAIN
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
#endif