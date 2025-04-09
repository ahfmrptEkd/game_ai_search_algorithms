// src/algorithms/single_player/with_context/beam.h
#ifndef BEAM_SEARCH_H
#define BEAM_SEARCH_H

#include "../../../games/maze/maze_state.h"
#include "../../../common/game_util.h"

struct BeamConfig {
    int search_width = GameConstants::AlgorithmParams::SEARCH_WIDTH;
    int search_depth = GameConstants::AlgorithmParams::SEARCH_DEPTH;
    int64_t time_threshold = GameConstants::AlgorithmParams::TIME_THRESHOLD; // ms
};

int beamSearchAction(const MazeState& state, const BeamConfig& config);

#endif // BEAM_SEARCH_H