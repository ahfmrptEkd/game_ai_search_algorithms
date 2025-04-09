#ifndef ITERATIVE_DEEPENING_H
#define ITERATIVE_DEEPENING_H

#include "../../../common/game_util.h"
#include "../../../games/twomaze/twomaze_state.h"

namespace iterative_deepening {
    ScoreType alphaBetaScore(const TwoMazeState& state, ScoreType alpha, ScoreType beta, int depth, const TimeKeeper& time_keeper);

    // 시간 제한이 있는 알파베타 탐색 행동 선택
    int alphaBetaActionWithTimeThreshold(const TwoMazeState& state, const int depth, const TimeKeeper& time_keeper);

    // 제한 시간내의 반복 깊이 우선 탐색
    int iterativeDeepeningAction(const TwoMazeState& state, const int64_t time_threshold);
}

int iterativeDeepeningSearchAction(const TwoMazeState& state, const int64_t time_threshold);

#endif // ITERATIVE_DEEPENING_H