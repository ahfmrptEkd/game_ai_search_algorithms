#ifndef RANDOM_ALGORITHM_H
#define RANDOM_ALGORITHM_H

#include "../../games/maze/maze_state.h"

// 무작위 행동 알고리즘 
int randomAction(const State& state);

// 시드를 고정해서 같은 게임 상황을 표시하면서 비교
void playGame(const int seed);

#endif // RANDOM_ALGORITHM_H 