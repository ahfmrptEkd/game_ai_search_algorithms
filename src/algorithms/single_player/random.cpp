#include "random.h"
#include <iostream>
#include <random>

std::mt19937 mt_for_action(0); // 난수 생성기

// 무작위 행동 알고리즘
int randomAction(const State& state)
{
    auto legal_actions = state.legalActions();
    return legal_actions[mt_for_action() % (legal_actions.size())];
}

// 시드를 고정해서 같은 게임 상황을 표시하면서 비교
void playGame(const int seed)
{
    auto state = State(seed);
    std::cout << state.toString() << std::endl;
    while (!state.isDone())
    {
        state.progress(randomAction(state));
        std::cout << state.toString() << std::endl;
    }
} 