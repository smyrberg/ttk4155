/*!
 * @file menu_items.h
 * @brief Menu content for the menu.
 */
#ifndef MENU_ITEMS_H_
#define MENU_ITEMS_H_

#include "game.h"

#define MENU_ITEMS 4 /*!< Max number of children in a menu item.*/



typedef struct menu_t menu_t;		// must declare type first so it can be self-referential

/*!
 * Structure for defining a node in a tree structure forming the menu.
 */
typedef struct menu_t
{
	char *name;						/*!< Name used for the menu. Also used as header.*/
	menu_t *parent;					/*!< Pointer to the parent of the menu.*/
	menu_t *children[MENU_ITEMS];	/*!< Pointer to any children of the menu.*/
	uint8_t child_count;			/*!< Number of child menus.*/
	void (*function_ptr)(void);		/*!< Function pointer for general functions. May be NULL if not present.*/
	game_score_t (*game_ptr)(void)	/*!< Game function pointer. May be NULL if menu is not a game.*/
};

menu_t m_main;					/*!< The root main menu. This menu is its own root*/
menu_t m_game;                  /*!< The top level game menu*/
menu_t m_quit;                  /*!< Quits the game. Does not actually do anything...*/
menu_t m_game_no_ctrl;          /*!< Game in no control mode*/
menu_t m_game_pid;              /*!< Game in PID mode*/
menu_t m_highscores;            /*!< Top level highscore menu*/
menu_t m_highscores_view;       /*!< Views the highscores*/
menu_t m_highscores_delete;     /*!< Deletes all highscores*/
menu_t m_settings;              /*!< Top level settings menu*/
menu_t m_settings_invert;       /*!< Setting for inverting the screen*/


#endif /* MENU_ITEMS_H_ */