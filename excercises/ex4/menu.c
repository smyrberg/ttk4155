#include "menu.h"
#include "oled.h"
#include "joystick.h"

#include <stdlib.h>

menu_item_info_t main_menu, play_game, highscores, settings, noob, basic, extreme, clear_highscores, contrast_level, invert_screen;

menu_item_info_t main_menu ={
	//.name = malloc(20),
	.name = "Main menu\n",
	.parent = NULL,
	.child[0] = &play_game,
	.child[1] = &highscores,
	.child[2] = &settings,
	.child_num = 3,
};

menu_item_info_t play_game = {
	.name = "Play game\n",
	.parent = &main_menu,
	.child[0] = &noob,
	.child[1] = &basic,
	.child[2] = &extreme,
	.child_num = 3,
};

menu_item_info_t highscores ={
	.name = "Highscores\n",
	.parent = &main_menu,
	.child[0] = NULL,
	.child_num = 0,
};

menu_item_info_t settings ={
	.name = "Settings\n",
	.parent = &main_menu,
	.child[0] = &clear_highscores,
	.child[1] = &contrast_level,
	.child[2] = &invert_screen,
	.child_num = 3,
};

menu_item_info_t noob = {
	.name = "N00b\n",
	.parent = &play_game,
	.child[0] = NULL,
	.child_num = 0,
};

menu_item_info_t basic = {
	.name = "Basic\n",
	.parent = &play_game,
	.child[0] = NULL,
	.child_num = 0,
};

menu_item_info_t extreme = {
	.name = "Extreme\n",
	.parent = &play_game,
	.child[0] = NULL,
	.child_num = 0,
};

menu_item_info_t clear_highscores ={
	.name = "Clear Highscores\n",
	.parent = &settings,
	.child[0] = NULL,
	.child_num = 0,
};

menu_item_info_t contrast_level ={
	.name = "Contrast level\n",
	.parent = &settings,
	.child[0] = NULL,
	.child_num = 0,
};

menu_item_info_t invert_screen ={
	.name = "Invert screen\n",
	.parent = &settings,
	.child[0] = NULL,
	.child_num = 0,
};

menu_item_info_t *current_menu;
menu_item_info_t *current_child;
int line;



void MENU_init( void ){
	current_menu = &main_menu;
	current_child = main_menu.child[0];
	line = 1;
	MENU_print_menu();
	MENU_highlight_item();
	while(1){
		MENU_select_item();
		MENU_navigate();
		//printf("Current line %d \n", line);
	}

}


void MENU_print_menu(){
	//printf("her\n");
	OLED_reset();
	OLED_print(current_menu->name, 0);
	//printf("her nå \n");
	if (current_child!= NULL){
		MENU_highlight_item();
	}
	for (int i =0; i < current_menu->child_num; i++){
		//printf("%d \n", current_menu->child_num);
		//printf("in for at child %d \n",i);
		OLED_goto_column(10);
		OLED_print(current_menu->child[i]->name,i+1);
	}
}


void MENU_select_item(){
	//int pressed = JOY_button();
	if (JOY_button() && current_child!= NULL){
		while(JOY_button()){}
		current_menu = current_child;
		current_child = current_child->child[0];
		line = 1;
		MENU_print_menu();
	}
}



void MENU_navigate(){
	//JOY_direction_t dir = JOY_get_direction();
	//printf("Direction: %d \n", dir);
	switch (JOY_get_direction()){
		case LEFT:
			while(JOY_get_direction() == LEFT){}
			//printf("LEFT - Current menu: %s \n", current_menu->name);
			if (current_menu->parent != NULL){
				line = 1;
				current_menu = current_menu->parent;
				current_child = current_menu->child[0];
				//printf("LEFT - Current line %d \n", line);
				MENU_print_menu();
			}
			break;
		case UP:
		while(JOY_get_direction() == UP){}
			//printf("UP - Current menu %s \n", current_menu->name);
			if (line > 1){
				line = line - 1;
				current_child = current_menu->child[line - 1];
				//printf("UP - Current line %d \n", line);
				MENU_highlight_item();
				}
			break;
		case DOWN:
		while(JOY_get_direction() == DOWN){}
			//printf("DOWN - Current menu %s \n", current_menu->name);
			if (line < current_menu->child_num){
				line = line + 1;
				current_child = current_menu->child[line - 1];
				//printf("DOWN - Current line %d \n", line);
				MENU_highlight_item();
				}
			break;
		
		default:
			break;
		
	}
}


void MENU_highlight_item( void )
{
	for (int i = 1; i < current_menu->child_num + 1; i++){
		OLED_pos(i, 0);
		OLED_print("  ",i);
	}
	
	OLED_pos(line, 0);
	OLED_print_arrow();
}