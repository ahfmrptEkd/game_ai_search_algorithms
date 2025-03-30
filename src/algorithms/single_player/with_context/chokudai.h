#ifndef CHOKUDAI_H
#define CHOKUDAI_H

#include "../../../games/maze/maze_state.h"
#include "../../../common/game_util.h"

struct ChokudaiConfig {
    int beam_width = 1;
    int beam_depth = 4;
    int beam_number = 2;
    int64_t time_threshold = 1;
};

int chokudaiSearchAction(const State& state, const ChokudaiConfig& config);

inline void playGameChokudai(const int seed, int64_t time_threshold = 1)
{
    playGameWithStrategy(seed, [time_threshold](const State& state) {
        ChokudaiConfig config;
        config.time_threshold = time_threshold;
        return chokudaiSearchAction(state, config);
    });
}


#endif // CHOKUDAI_H