#ifndef MINIMAX_H
#define MINIMAX_H

#include "../../../games/twomaze/twomaze_state.h"
#include "../../../common/game_util.h"

namespace minimax {
    // minimax 알고리즘을 점수 계산
    ScoreType miniMaxScore(const TwoMazeState& state, const int depth);

    // minimax 알고리즘을 행동 결정
    int miniMaxAction(const TwoMazeState& state, const int depth);
}

// 특정 깊이의 탐색을 하는 함수
int miniMaxSearchAction(const TwoMazeState& state, const int depth);

inline void playGameMinimax(const int seed, int depth = GameConstants::AlgorithmParams::SEARCH_DEPTH) {
    playGameWithStrategy<TwoMazeState>(seed, [depth] (const TwoMazeState& state) 
    {
        return miniMaxSearchAction(state, depth);
    });
}
#endif // MINIMAX_H
