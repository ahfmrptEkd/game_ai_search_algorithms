#ifndef GAME_UTIL_H
#define GAME_UTIL_H

#include <iostream>
#include <functional>
#include <random>
#include <chrono>
#include "../games/maze/maze_state.h"

class TimeKeeper 
{
private:
    std::chrono::high_resolution_clock::time_point start_time_;
    int64_t time_threshold_;
public:
    TimeKeeper(const int64_t &time_threshold)
        : start_time_(std::chrono::high_resolution_clock::now()),
          time_threshold_(time_threshold)
    {
    }
    
    bool isTimeOver() const
    {
        auto diff = std::chrono::high_resolution_clock::now() - this->start_time_;
        return std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() >= time_threshold_;
    }
};

// 모든 알고리즘에서 공통으로 사용할 유틸리티를 위한 네임스페이스
namespace GameUtil {
    extern std::mt19937 mt_for_action;
}

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