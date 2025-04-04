#ifndef HILLCLIMB_ALGORITHM_H
#define HILLCLIMB_ALGORITHM_H

#include "../../../games/automaze/automaze_state.h"
#include "random.h"

AutoMazeState hillClimbPlacement(const AutoMazeState &state, int number);

inline void playGameHillClimb(const int seed, int number = GameConstants::AlgorithmParams::SEARCH_NUMBER) {
    playAutoMazeGame([number](const AutoMazeState& state) { 
        return hillClimbPlacement(state, number); 
        }, seed);
}

#endif // HILLCLIMB_ALGORITHM_H