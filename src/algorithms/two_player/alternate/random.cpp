// src/algorithms/two_player/alternating/random.cpp
#include "random.h"
#include "../../../common/game_util.h"
#include <iostream>
#include <random>

// 무작위 행동 알고리즘
int randomAction(const TwoMazeState& state)
{
    auto legal_actions = state.legalActions();
    if (legal_actions.empty())
    {
        return 0;
    }
    return legal_actions[GameUtil::mt_for_action() % (legal_actions.size())];
}