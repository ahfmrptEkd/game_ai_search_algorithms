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

void playSimMazeGame(
    const std::function<int(const SimMazeState&, const int)>& action0_func,
    const std::function<int(const SimMazeState&, const int)>& action1_func,
    const int seed)
{
    auto state = SimMazeState(seed);
    
    while (!state.isDone()) {
        std::cout << state.toString() << std::endl;
        
        int action0 = action0_func(state, 0);
        int action1 = action1_func(state, 1);
        
        std::cout << "actions: " 
                  << "Player 0: " << direction_strings[action0] << " "
                  << "Player 1: " << direction_strings[action1] << std::endl;
        
        state.advance(action0, action1);
    }
    
    std::cout << state.toString() << std::endl;
    
    auto status = state.getWinningStatus();
    printGameResult(status);
}