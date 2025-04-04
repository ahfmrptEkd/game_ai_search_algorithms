#include "pmc.h"
#include <vector>
#include <algorithm>

namespace sim_pmc {
    double playout(SimMazeState* state) {
        if (state->isDone()) {
            WinningStatus status = state->getWinningStatus();
            switch (status) {
                case WinningStatus::WIN:
                    return 1.0;
                case WinningStatus::LOSE:
                    return 0.0;
                case WinningStatus::DRAW:
                    return 0.5;
                default:
                    return 0.5;
            }
        }
        
        // 게임이 진행 중이면 랜덤 행동으로 다음 상태 생성
        int action0 = simMazeRandomAction(*state, 0);
        int action1 = simMazeRandomAction(*state, 1);
        state->advance(action0, action1);
        
        // 재귀적으로 플레이아웃 계속
        return playout(state);
    }

    int primitiveMontecarloAction(const SimMazeState& state, const int player_id, const int playout_number) {
        auto my_legal_actions = state.legalActions(player_id);
        auto opp_legal_actions = state.legalActions((player_id + 1) % 2);
        
        std::vector<double> action_values(my_legal_actions.size(), 0.0);
        
        // 각 행동에 대해 플레이아웃 수행
        for (size_t i = 0; i < my_legal_actions.size(); i++) {
            int my_action = my_legal_actions[i];
            
            // 지정된 횟수만큼 시뮬레이션
            for (int j = 0; j < playout_number; j++) {
                int opp_action = opp_legal_actions[GameUtil::mt_for_action() % opp_legal_actions.size()];
                
                SimMazeState next_state = state;
                if (player_id == 0) {
                    next_state.advance(my_action, opp_action);
                } else {
                    next_state.advance(opp_action, my_action);
                }
                
                double player0_win_rate = playout(&next_state);
                double win_rate = (player_id == 0) ? player0_win_rate : (1.0 - player0_win_rate);
                action_values[i] += win_rate;
            }
        }
        
        // 가장 가치가 높은 행동 선택
        int best_action_index = std::distance(action_values.begin(), 
                                             std::max_element(action_values.begin(), action_values.end()));
        
        return my_legal_actions[best_action_index];
    }
}

int pmcSearchAction(const SimMazeState& state, const int player_id, int simulation_number) {
    return sim_pmc::primitiveMontecarloAction(state, player_id, simulation_number);
}