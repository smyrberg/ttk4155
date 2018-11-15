#ifndef GAME_H_
#define GAME_H_

#include <stdio.h>

typedef struct {
	uint8_t		failures;
	uint16_t	time;
	uint8_t		valid;
} game_score_t;


game_score_t GAME_play();
game_score_t GAME_texas();


#endif /* GAME_H_ */