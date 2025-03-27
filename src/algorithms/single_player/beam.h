#ifndef BEAM_H
#define BEAM_H

#include "../../games/maze/maze_state.h"
#include "../../common/game_util.h"

// 빔 탐색으로 행동을 결정한다.
int beamSearchAction(const State& state, const int beam_width, const int beam_depth);

inline void playGameBeam(const int seed, const int beam_width = 2, const int beam_depth = 4)
{
    playGameWithStrategy(seed, [beam_width, beam_depth](const State& state) {
        return beamSearchAction(state, beam_width, beam_depth);
    });
}

#endif // BEAM_H