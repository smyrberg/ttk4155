#ifndef SCORES_H_
#define SCORES_H_

#define USERNAME_CHAR_COUNT 10
#define NUM_SCORES 5

#include <stdio.h>


typedef struct SCORE_info_t 
{
	uint16_t score;
	char name[USERNAME_CHAR_COUNT];
} SCORE_info_t;


void SCORE_delete();



#endif /* SCORES_H_ */