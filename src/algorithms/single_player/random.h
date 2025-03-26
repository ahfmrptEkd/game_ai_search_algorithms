#ifndef RANDOM_ALGORITHM_H
#define RANDOM_ALGORITHM_H

#include "../../games/maze/maze_state.h"
#include "../../common/game_util.h"

// 무작위 행동 알고리즘 
int randomAction(const State& state);

// 시드를 고정해서 게임을 무작위 알고리즘으로 진행
inline void playGameRandom(const int seed) {
    playGameWithStrategy(seed, randomAction);
}

#endif // RANDOM_ALGORITHM_H 