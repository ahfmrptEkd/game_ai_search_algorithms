#ifndef CHOKUDAI_H
#define CHOKUDAI_H

#include "../../games/maze/maze_state.h"
#include "../../common/game_util.h"

struct ChokudaiConfig {
    int beam_width = 1;
    int beam_depth = 4;
    int beam_number = 2;
};

int chokudaiSearchAction(const State& state, const ChokudaiConfig& config);

inline void playGameChokudai(const int seed)
{
    playGameWithStrategy(seed, [](const State& state) {
        return chokudaiSearchAction(state, ChokudaiConfig{});
    });
}


#endif // CHOKUDAI_H