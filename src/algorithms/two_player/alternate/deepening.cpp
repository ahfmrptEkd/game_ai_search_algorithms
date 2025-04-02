#include "deepening.h"
#include "../../../common/game_util.h"
#include <iostream>

namespace iterative_deepening {

    ScoreType alphaBetaScore(const TwoMazeState& state, ScoreType alpha, ScoreType beta, int depth, const TimeKeeper& time_keeper) {
        if (time_keeper.isTimeOver()) return 0;

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
            ScoreType score = -alphaBetaScore(next_state, -beta, -alpha, depth - 1, time_keeper);

            if (score > alpha) {
                alpha = score;
            }

            if (alpha >= beta) {
                return alpha;
            }
        }

        return alpha;
    }

    int alphaBetaActionWithTimeThreshold(const TwoMazeState& state, const int depth, const TimeKeeper& time_keeper) {
        int best_action = -1;
        ScoreType alpha = -GameConstants::INF;
        ScoreType beta = GameConstants::INF;

        for (const auto& action : state.legalActions()) {
            TwoMazeState next_state = state;
            next_state.progress(action);
            ScoreType score = -alphaBetaScore(next_state, -beta, -alpha, depth - 1, time_keeper);

            if (score > alpha) {
                best_action = action;
                alpha = score;
            }
        }

        return best_action != -1 ? best_action : 
                                    (state.legalActions().empty() ? -1 : state.legalActions()[0]);
    }

    int iterativeDeepeningAction(const TwoMazeState& state, int64_t time_threshold) {
        TimeKeeper time_keeper(time_threshold);
        int best_action = -1;

        if (state.legalActions().empty()) {
            return -1;
        }

        // 최소 깊이 1 보장을 위한 기본값 설정
        best_action = state.legalActions()[0];

        for (int depth = 1; depth <= GameConstants::Board::END_TURN * 2; ++depth) {     // 시간 제한이 있으므로 최대 깊이 설정 알파와 베타의 최대 턴 수
            int action = alphaBetaActionWithTimeThreshold(state, depth, time_keeper);

            if (time_keeper.isTimeOver()) {
                break;
            }

            best_action = action;
        }

        return best_action;
    }
}

int iterativeDeepeningSearchAction(const TwoMazeState& state, const int64_t time_threshold) {
    return iterative_deepening::iterativeDeepeningAction(state, time_threshold);
}