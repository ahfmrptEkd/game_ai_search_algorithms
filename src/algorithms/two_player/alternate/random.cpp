// src/algorithms/two_player/alternating/random.cpp
#include "random.h"
#include "../../../common/game_util.h"
#include <iostream>
#include <random>

// 무작위 행동 알고리즘
int twoMazeRandomAction(const TwoMazeState& state)
{
    auto legal_actions = state.legalActions();
    if (legal_actions.empty())
    {
        return 0;
    }
    return legal_actions[GameUtil::mt_for_action() % (legal_actions.size())];
}

void playTwoMazeGame(const std::function<int(const TwoMazeState&)>& action_func, const int seed)
{
    auto state = TwoMazeState(seed);
    
    while (!state.isDone()) {
        std::cout << state.toString() << std::endl;
        auto action = action_func(state);
        state.progress(action);
    }
    
    // 마지막 상태 출력
    std::cout << state.toString() << std::endl;
    
    // 게임 종료 후 결과 출력
    auto status = state.getWinningStatus();
    printGameResult(status);
}