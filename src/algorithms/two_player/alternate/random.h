// src/algorithms/two_player/alternating/random.h
#ifndef TWOPLAYER_RANDOM_H
#define TWOPLAYER_RANDOM_H

#include "../../../games/twomaze/twomaze_state.h"
#include "../../../common/game_util.h"

// 무작위 행동 알고리즘 
int randomAction(const TwoMazeState& state);

// 시드를 고정해서 게임을 무작위 알고리즘으로 진행
inline void playGameTwoMazeRandom(const int seed) {
    playGameWithStrategy<TwoMazeState>(seed, randomAction);
}

#endif // TWOPLAYER_RANDOM_H