menu_t m_main = 
{
	.name="Main Menu",
	.parent=&m_main,
	.children=
	{
		&m_game,
		&m_highscores,
		&m_settings,
		&m_quit
	}, 
	.child_count=4, 
	.function_ptr=NULL,
	.game_ptr=NULL
};

menu_t m_game= 
{
	.name="Games",
	.parent=&m_main,
	.children=
	{
		&m_game_no_ctrl,
		&m_game_pid
	},
	.child_count=2,
	.function_ptr=NULL,
	.game_ptr=NULL
};

menu_t m_game_no_ctrl= 
{
	.name="No Control",
	.parent=&m_game,
	.children=NULL,
	.child_count=0,
	.function_ptr=NULL,
	.game_ptr=&GAME_no_ctrl
};

menu_t m_game_pid= 
{
	.name="PID",
	.parent=&m_game,
	.children=NULL,
	.child_count=0,
	.function_ptr=NULL,
	.game_ptr=&GAME_pid
};

menu_t m_quit=
{
	.name="Quit",
	.parent=&m_main,
	.children=NULL,
	.child_count=0,
	.function_ptr=NULL,
	.game_ptr=NULL
};

menu_t m_highscores=
{
	.name="Highscores",
	.parent=&m_main,
	.children=
	{
		&m_highscores_view,
		&m_highscores_delete
	}, 
	.child_count=2,
	.function_ptr=NULL,
	.game_ptr=NULL
};

menu_t m_highscores_view = 
{
	.name="View Highscores",
	.parent=&m_highscores,
	.children=NULL,
	.child_count=0,
	.function_ptr=&view_highscores,
	.game_ptr=NULL
};

menu_t m_highscores_delete = 
{
	.name="Delete Highscores",
	.parent=&m_highscores,
	.children=NULL,
	.child_count=0,
	.function_ptr=&delete_highscores,
	.game_ptr=NULL
};

menu_t m_settings = 
{
	.name="Settings",
	.parent=&m_main, 
	.children=
	{
		&m_settings_invert
	}, 
	.child_count=1, 
	.function_ptr=NULL,
	.game_ptr=NULL
};

menu_t m_settings_invert = 
{
	.name="Invert Screen",
	.parent=&m_settings,
	.children=NULL,
	.child_count=0,
	.function_ptr=&OLED_invert_screen,
	.game_ptr=NULL
};

