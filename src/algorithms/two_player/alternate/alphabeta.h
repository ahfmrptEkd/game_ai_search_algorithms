#ifndef ALPHABETA_H
#define ALPHABETA_H

#include "../../../common/game_util.h"
#include "../../../games/twomaze/twomaze_state.h"

namespace alphabeta {
    ScoreType alphaBetaScore(const TwoMazeState& state, ScoreType alpha, ScoreType beta, const int depth);

    int alphaBetaAction(const TwoMazeState& state, const int depth);
}

int alphaBetaSearchAction(const TwoMazeState& state, const int depth);

#endif // ALPHABETA_H