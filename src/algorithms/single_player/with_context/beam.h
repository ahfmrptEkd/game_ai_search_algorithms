#ifndef BEAM_SEARCH_H
#define BEAM_SEARCH_H

#include "../../../games/maze/maze_state.h"
#include "../../../common/game_util.h"

struct BeamConfig {
    int beam_width = 5;
    int beam_depth = 10;
    int64_t time_threshold = 1; // ms
};

// 빔 탐색으로 행동을 결정한다.
int beamSearchAction(const MazeState& state, const BeamConfig& config);

// 기본 설정을 사용하는 함수
inline void playGameBeam(const int seed, int64_t time_threshold = 1)
{
    playGameWithStrategy<MazeState>(seed, [time_threshold](const MazeState& state) {
        BeamConfig config;
        config.time_threshold = time_threshold;
        return beamSearchAction(state, config);
    });
}

#endif // BEAM_SEARCH_H