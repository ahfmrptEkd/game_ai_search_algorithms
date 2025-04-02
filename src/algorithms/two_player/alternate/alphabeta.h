#ifndef ALPHABETA_H
#define ALPHABETA_H

#include "../../../common/game_util.h"
#include "../../../games/twomaze/twomaze_state.h"
#include "random.h"

namespace alphabeta {
    // 알파베타 가지치기 점수 계산
    ScoreType alphaBetaScore(const TwoMazeState& state, ScoreType alpha, ScoreType beta, const int depth);

    // 알파베타 가지치기 행동 선택
    int alphaBetaAction(const TwoMazeState& state, const int depth);
}

int alphaBetaSearchAction(const TwoMazeState& state, const int depth);

inline void playGameAlphaBeta(const int seed, int depth = GameConstants::AlgorithmParams::SEARCH_DEPTH) {
    playGame([depth](const TwoMazeState& state){
        return alphaBetaSearchAction(state, depth);
    }, seed);
}
#endif // ALPHABETA_H