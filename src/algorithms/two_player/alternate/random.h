// src/algorithms/two_player/alternating/random.h
#ifndef TWOPLAYER_RANDOM_H
#define TWOPLAYER_RANDOM_H

#include "../../../games/twomaze/twomaze_state.h"
#include "../../../common/game_util.h"

int twoMazeRandomAction(const TwoMazeState& state);

void playTwoMazeGame(const std::function<int(const TwoMazeState&)>& action_func, const int seed);

// 시드를 고정해서 게임을 무작위 알고리즘으로 진행
inline void playTwoMazeGameRandom(const int seed) {
    playTwoMazeGame([](const TwoMazeState& state) {
        return twoMazeRandomAction(state);
    }, seed);
}
#endif // TWOPLAYER_RANDOM_H