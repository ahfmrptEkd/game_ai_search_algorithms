// src/algorithms/two_player/alternate/minimax.cpp
#include "minimax.h"
#include "../../../common/game_util.h"
#include "../../../common/game_state.h"
#include <iostream>

namespace minimax {
    ScoreType miniMaxScore(const TwoMazeState& state, const int depth) {
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

        ScoreType bestScore = -GameConstants::INF;
        for (const auto action : legal_actions) {
            TwoMazeState next_state = state;
            next_state.progress(action);
            // 여기가 중요: 상대방 시점에서는 최소화하는 것이 우리에게 최선이므로 부호를 반전
            ScoreType score = -miniMaxScore(next_state, depth - 1);
            if (score > bestScore) {
                bestScore = score;
            }
        }
        return bestScore;
    }
    
    // minimax 알고리즘을 행동 결정
    int miniMaxAction(const TwoMazeState& state, const int depth) {
        ScoreType best_action = -1;
        ScoreType best_score = -GameConstants::INF;

        // 가능한 모든 행동에 대해 평가
        for (const auto action : state.legalActions()) {
            TwoMazeState next_state = state;
            next_state.progress(action);
            // 여기도 마찬가지: 상대 시점에서의 최소화
            ScoreType score = -miniMaxScore(next_state, depth - 1);

            if (score > best_score) {
                best_action = action;
                best_score = score;
            }
        }
        return best_action;
    }
};

int miniMaxSearchAction(const TwoMazeState& state, int depth) {
    return minimax::miniMaxAction(state, depth);
}