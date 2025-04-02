#include "alphabeta.h"
#include "../../../common/game_util.h"
#include <iostream>

namespace alphabeta {
    ScoreType alphaBetaScore(const TwoMazeState& state, ScoreType alpha, ScoreType beta, const int depth) {
        if (state.isDone() || depth == 0) {
            TwoMazeState state_copy = state;
            state_copy.evaluateScore();
            return state_copy.evaluated_score_;
        }

        auto legal_actions = state.legalActions();
        if (legal_actions.empty()) {
            TwoMazeState state_copy = state;
            state_copy.evaluateScore();
            return state_copy.evaluated_score_;
        }

        for (const auto& action : legal_actions) {
            TwoMazeState next_state = state;
            next_state.progress(action);

            // 다음 플레이어 시점에서는 최소화함으로 부호 반전
            ScoreType score = -alphaBetaScore(next_state, -beta, -alpha, depth - 1);

            if (score > alpha) {
                alpha = score;
            }

            // beta 컷 오프 - 베타 값 이상의 노드를 탐색할 필요가 없음
            if (alpha >= beta) {
                return alpha;
            }
        }
        return alpha;
    }

    int alphaBetaAction(const TwoMazeState& state, const int depth) {
        int best_action = -1;
        ScoreType alpha = -GameConstants::INF;
        ScoreType beta = GameConstants::INF;

        for (const auto& action : state.legalActions()) {
            TwoMazeState next_state = state;
            next_state.progress(action);
            ScoreType score = alphaBetaScore(next_state, alpha, beta, depth - 1);

            if (score > alpha) {
                best_action = action;
                alpha = score;
            }
        }
        return best_action;
    }
};

int alphaBetaSearchAction(const TwoMazeState& state, const int depth) {
    return alphabeta::alphaBetaAction(state, depth);
}