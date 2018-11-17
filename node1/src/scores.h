#ifndef SCORES_H_
#define SCORES_H_

#define USERNAME_CHAR_COUNT 10
#define NUM_SCORES 5

#include <stdio.h>

typedef struct {
	uint8_t		failures;
	uint16_t	time_ms;
	uint8_t		valid;
} game_score_t;



void SCORES_add(game_score_t score);
void SCORES_delete();
void SCORES_view();



#endif /* SCORES_H_ */