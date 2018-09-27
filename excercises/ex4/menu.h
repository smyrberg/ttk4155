#ifndef MENU_H_
#define MENU_H_

typedef struct menu_item_info menu_item_info_t;

	
typedef struct menu_item_info {
	char *name;
	menu_item_info_t *parent;
	menu_item_info_t *child[7];
	int child_num;
};


void MENU_init( void );

void MENU_print_menu();

void MENU_select_item();

void MENU_navigate();

void MENU_highlight_item( void );



#endif /* MENU_H_ */