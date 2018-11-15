#ifndef GAME_H_
#define GAME_H_

#include <stdio.h>

typedef struct {
	uint8_t		failures;
	uint16_t	time_ms;
	uint8_t		valid;
} game_score_t;


game_score_t GAME_no_ctrl();
game_score_t GAME_pid();


#endif /* GAME_H_ */