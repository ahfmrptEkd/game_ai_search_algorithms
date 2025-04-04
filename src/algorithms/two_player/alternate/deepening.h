#ifndef ITERATIVE_DEEPENING_H
#define ITERATIVE_DEEPENING_H

#include "../../../common/game_util.h"
#include "../../../games/twomaze/twomaze_state.h"
#include "random.h"

namespace iterative_deepening {
    // 시간 제한이 있는 알파베타 탐색
    ScoreType alphaBetaScore(const TwoMazeState& state, ScoreType alpha, ScoreType beta, int depth, const TimeKeeper& time_keeper);

    // 시간 제한이 있는 알파베타 탐색 행동 선택
    int alphaBetaActionWithTimeThreshold(const TwoMazeState& state, const int depth, const TimeKeeper& time_keeper);

    // 제한 시간내의 반복 깊이 우선 탐색
    int iterativeDeepeningAction(const TwoMazeState& state, const int64_t time_threshold);
}

int iterativeDeepeningSearchAction(const TwoMazeState& state, const int64_t time_threshold);


inline void playGameIterativeDeepening(const int seed, const int64_t time_threshold = 100) {
    playTwoMazeGame([time_threshold](const TwoMazeState& state) {
        return iterativeDeepeningSearchAction(state, time_threshold);
    }, seed);
}


#endif // ITERATIVE_DEEPENING_H