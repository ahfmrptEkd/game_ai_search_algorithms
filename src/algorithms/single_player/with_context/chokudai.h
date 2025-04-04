// src/algorithms/single_player/with_context/chokudai.h
#ifndef CHOKUDAI_H
#define CHOKUDAI_H

#include "../../../games/maze/maze_state.h"
#include "../../../common/game_util.h"

struct ChokudaiConfig {
    int search_width = GameConstants::AlgorithmParams::SEARCH_WIDTH;
    int search_depth = GameConstants::AlgorithmParams::SEARCH_DEPTH;
    int search_number = GameConstants::AlgorithmParams::SEARCH_NUMBER;
    int64_t time_threshold = GameConstants::AlgorithmParams::TIME_THRESHOLD;
};

int chokudaiSearchAction(const MazeState& state, const ChokudaiConfig& config);

inline void playMazeGameChokudai(const int seed, int64_t time_threshold = 1) {
    playGameWithStrategy<MazeState>(seed, [time_threshold](const MazeState& state) {
        ChokudaiConfig config;
        config.time_threshold = time_threshold;
        return chokudaiSearchAction(state, config);
    });
}

#endif // CHOKUDAI_H