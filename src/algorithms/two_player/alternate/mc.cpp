#include "mc.h"
#include "random.h"
#include "../../../common/game_util.h"
#include <iostream>

namespace monte_carlo {
    double playout(TwoMazeState state) {
        WinningStatus status = state.getWinningStatus();

        if (status == WinningStatus::WIN) {
            return 1.0;
        } else if (status == WinningStatus::LOSE) {
            return 0.0;
        } else {
            return 0.5;   
        }

        // 랜덤 플레이 아웃
        int action = twoMazeRandomAction(state);
        state.progress(action);

        // 재귀적으로 다음 상태에서 플레이아웃 진행 (상대방 시점이므로 1-value 반환)
        return 1.0 - playout(state);

    }

    int monteCarloAction(const TwoMazeState& state, const int playout_number) {
        auto legal_actions = state.legalActions();
        if (legal_actions.empty()) {
            return -1;
        }

        std::vector<double> values(legal_actions.size(), 0.0);
        std::vector<int> counts(legal_actions.size(), 0);

        for (int i = 0; i < playout_number; i++) {
            int action_index = i % legal_actions.size();
            int action = legal_actions[action_index];

            TwoMazeState next_state = state;
            next_state.progress(action);

            double win_rate = 1.0 - playout(next_state);
            values[action_index] += win_rate;
            counts[action_index]++;
        }

        // 최선의 행동 선택
        int best_action_index = 0;
        double best_value = -1.0;

        for (size_t i = 0; i < legal_actions.size(); i++) {
            if (counts[i] > 0) {
                double average_value = values[i] / counts[i];
                if (average_value > best_value) {
                    best_value = average_value;
                    best_action_index = i;
                }
            }
        }

        return legal_actions[best_action_index];
    }
}

int monteCarloSearchAction(const TwoMazeState& state, const int playout_number) {
    return monte_carlo::monteCarloAction(state, playout_number);
}