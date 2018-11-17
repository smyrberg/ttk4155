#include "scores.h"
#include "drivers/oled.h"
#include "drivers/joystick.h"
#include <avr/interrupt.h>
#include <avr/delay.h>

#define NUM_SCORES 5
static game_score_t g_highscores[NUM_SCORES];

void SCORES_add(game_score_t score)
{
	for (int i = 0; i < NUM_SCORES; i++)
	{
		if (score.failures < g_highscores[i].failures || !g_highscores[i].valid)
		{
			for (int j = NUM_SCORES-1; j > i; j--)
			{
				g_highscores[j] = g_highscores[j-1];
			}
			g_highscores[i] = score;
			return;
		}
	}
}

static void print_score(int pos, game_score_t s)
{
	if (s.valid)
	{
		OLED_printf("%d. Time: %02d Fail: %02d",pos, s.time_ms, s.failures);
	}
	else {
		OLED_printf("%d. BLANK", pos);
	}
}


void SCORES_delete()
{
	OLED_reset();
	OLED_printf("deleting highscores...");
	memset(g_highscores, 0, NUM_SCORES * sizeof(game_score_t));
	_delay_ms(1000);
	return;
}


void SCORES_view()
{
	OLED_reset();
	OLED_pos(0, ARROW_WIDTH);
	OLED_printf("Highscore List");
	for (int i = 1;i <= NUM_SCORES; i++)
	{
		OLED_pos(i, ARROW_WIDTH);
		print_score(i, g_highscores[i-1]);
	}
		
	OLED_pos(7, 0);
	OLED_print_back_arrow();
		
	while(JOY_get_4axis_direction() != menu_left);
}


#if 0
static void EEPROM_write(unsigned int addr, unsigned char data)
{
	// Wait for completion of previous write
	while(EECR & (1<<EEWE));
	
	// Set up address and data registers
	EEAR = addr;
	EEDR = data;
	
	// Write logical one to EEMWE
	EECR |= (1<<EEMWE);
	
	// Start eeprom write by setting EEWE
	EECR |= (1<<EEWE);
}

unsigned char EEPROM_read(unsigned int addr)
{
	// Wait for completion of previous write
	while(EECR & (1<<EEWE));
	
	// Set up address register
	EEAR = addr;
	
	// Start eeprom read by writing EERE
	EECR |= (1<<EERE);
	
	// Return data from data register
	return EEDR;
}

void SCORE_delete()
{
	for (int i=0; i < sizeof(SCORE_info_t) * NUM_SCORES; i++ )
	{
		EEPROM_write(i, 0);
	}
}

#endif