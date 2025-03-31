#include "greedy.h"
#include "../../../common/game_util.h"
#include <iostream>
#include <random>

// 탐욕법으로 행동을 결정한다.
int greedyAction(const MazeState& state)
{
    auto legal_actions = state.legalActions();
    ScoreType best_score = -GameConstants::INF; 
    int best_action = -1;
    for (const auto action : legal_actions)
    {
        MazeState now_state = state;
        now_state.progress(action);
        now_state.evaluateScore();
        if (now_state.evaluated_score_ > best_score)
        {
            best_score = now_state.evaluated_score_;
            best_action = action;
        }
    }
    return best_action;
}