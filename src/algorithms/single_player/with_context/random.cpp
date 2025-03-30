#include "random.h"
#include "../../../common/game_util.h"
#include <iostream>
#include <random>

// 무작위 행동 알고리즘
int randomAction(const State& state)
{
    auto legal_actions = state.legalActions();
    return legal_actions[mt_for_action() % (legal_actions.size())];
} 