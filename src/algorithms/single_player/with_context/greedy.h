// src/algorithms/single_player/with_context/greedy.h
#ifndef GREEDY_ALGORITHM_H
#define GREEDY_ALGORITHM_H

#include "../../../games/maze/maze_state.h"
#include "../../../common/game_util.h"

// 탐욕법으로 행동을 결정한다.
int greedyAction(const MazeState& state);

#endif // GREEDY_ALGORITHM_H