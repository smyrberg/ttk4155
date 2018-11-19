/*!
 * @file game.h
 * @brief Functions for running games on node 1
 */
#ifndef GAME_H_
#define GAME_H_

#include <stdio.h>

#include "scores.h"

/*!
 * Run the game in no control mode.
 * @return The score achieved. (number of times the IR beam has been broken during the game)
 */
game_score_t GAME_no_ctrl();

/*!
 * Run the game in position control mode.
 * @return The score achieved. (number of times the IR beam has been broken during the game)
 */
game_score_t GAME_pos_control();


#endif /* GAME_H_ */