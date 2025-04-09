// src/algorithms/two_player/alternate/minimax.h
#ifndef MINIMAX_H
#define MINIMAX_H

#include "../../../games/twomaze/twomaze_state.h"
#include "../../../common/game_util.h"

namespace minimax {
    ScoreType miniMaxScore(const TwoMazeState& state, const int depth);

    int miniMaxAction(const TwoMazeState& state, const int depth);
}

int miniMaxSearchAction(const TwoMazeState& state, const int depth);

#endif // MINIMAX_H
