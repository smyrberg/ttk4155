#ifndef MENU_ITEMS_H_
#define MENU_ITEMS_H_

#include "game.h"

#define MENU_ITEMS 4

// must declare type first so it can be self-referential
typedef struct menu_t menu_t;
typedef struct menu_t
{
	char *name;
	menu_t *parent;
	menu_t *children[MENU_ITEMS];
	uint8_t child_count;
	void (*function_ptr)(void);
	game_score_t (*game_ptr)(void)
};

menu_t m_main, m_game, m_quit;
menu_t m_game_no_ctrl, m_game_pid;
menu_t m_highscores, m_highscores_view, m_highscores_delete;
menu_t m_settings, m_settings_invert;


#endif /* MENU_ITEMS_H_ */