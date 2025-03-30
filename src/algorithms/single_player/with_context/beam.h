#ifndef BEAM_H
#define BEAM_H

#include "../../../games/maze/maze_state.h"
#include "../../../common/game_util.h"

struct BeamConfig {
    int beam_width = 2;
    int beam_depth = 4;
    int64_t time_threshold = 1; // 시간 제한 0은 제한 없음 의미; 1은 1밀리초 제한
};

// 빔 탐색으로 행동을 결정한다.
int beamSearchAction(const State& state, const BeamConfig& config);

// 기본 설정을 사용하는 함수
inline void playGameBeam(const int seed, int64_t time_threshold = 1)
{
    playGameWithStrategy(seed, [time_threshold](const State& state) {
        BeamConfig config;
        config.time_threshold = time_threshold;
        return beamSearchAction(state, config);
    });
}

#endif // BEAM_H