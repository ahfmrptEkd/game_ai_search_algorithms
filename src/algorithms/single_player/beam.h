#ifndef BEAM_H
#define BEAM_H

#include "../../games/maze/maze_state.h"
#include "../../common/game_util.h"

struct BeamConfig {
    int beam_width = 2;
    int beam_depth = 4;
};

// 빔 탐색으로 행동을 결정한다.
int beamSearchAction(const State& state, const BeamConfig& config);

// 기본 설정을 사용하는 함수
inline void playGameBeam(const int seed)
{
    playGameWithStrategy(seed, [](const State& state) {
        return beamSearchAction(state, BeamConfig{});
    });
}

#endif // BEAM_H