/*!
 * @file scores.h
 * @brief Highscore handeling.
 */
#ifndef SCORES_H_
#define SCORES_H_

#include <stdio.h>

/*!
 * Structure holding the data from one game.
 */
typedef struct {
	uint8_t		failures;		/*!< Number of times the IR beam has been broken*/
	uint16_t	time_ms;		/*!< Time the game took*/
	uint8_t		valid;			/*!< If the game score entry is valid. Valid if non-zero*/
} game_score_t;

/*!
 * Add score to the score list.
 * @param[in] score Score to add.
 */
void SCORES_add(game_score_t score);

/*!
 * Delete all scores.
 */
void SCORES_delete();

/*!
 * Print score list to the OLED.
 */
void SCORES_view();


#endif /* SCORES_H_ */