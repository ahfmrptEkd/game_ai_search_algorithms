#ifndef GAME_UTIL_H
#define GAME_UTIL_H

#include <iostream>
#include <functional>
#include <random>
#include "../games/maze/maze_state.h"

// 모든 알고리즘에서 공통으로 사용할 난수 생성기
extern std::mt19937 mt_for_action;

// 알고리즘 전략을 매개변수로 받아 게임을 진행한다
inline void playGameWithStrategy(const int seed, std::function<int(const State&)> strategy_func) {
    auto state = State(seed);
    std::cout << state.toString() << std::endl;
    while (!state.isDone()) {
        state.progress(strategy_func(state));
        std::cout << state.toString() << std::endl;
    }
}

#endif // GAME_UTIL_H 