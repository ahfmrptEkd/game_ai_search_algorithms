#include "random.h"
#include "../../../common/game_util.h"
#include <iostream>
#include <random>

int simMazeRandomAction(const SimMazeState& state, const int player_id)
{
    auto legal_actions = state.legalActions(player_id);
    if (legal_actions.empty())
    {
        return 0;
    }
    return legal_actions[GameUtil::mt_for_action() % (legal_actions.size())];
}

// (디버깅/출력용)
static const std::string direction_strings[4] = {"RIGHT", "LEFT", "DOWN", "UP"};