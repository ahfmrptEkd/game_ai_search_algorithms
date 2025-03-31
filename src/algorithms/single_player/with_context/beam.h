#ifndef BEAM_SEARCH_H
#define BEAM_SEARCH_H

#include "../../../games/maze/maze_state.h"
#include "../../../common/game_util.h"

struct BeamConfig {
    int search_width = GameConstants::AlgorithmParams::SEARCH_WIDTH;
    int search_depth = GameConstants::AlgorithmParams::SEARCH_DEPTH;
    int64_t time_threshold = GameConstants::AlgorithmParams::TIME_THRESHOLD; // ms
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