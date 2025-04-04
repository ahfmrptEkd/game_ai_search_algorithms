#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include "../../../games/automaze/automaze_state.h"
#include "../../../common/game_util.h"
#include "random.h"

AutoMazeState simulatedAnnealingPlacement(const AutoMazeState &state, int number, double start_temp, double end_temp);


inline void playGameSimulatedAnnealing(const int seed, int number = GameConstants::AlgorithmParams::SEARCH_NUMBER, double start_temp = GameConstants::AlgorithmParams::START_TEMPERATURE, double end_temp = GameConstants::AlgorithmParams::END_TEMPERATURE) {
    playAutoMazeGame([number, start_temp, end_temp](const AutoMazeState& state) {
        return simulatedAnnealingPlacement(state, number, start_temp, end_temp);
    }, seed);
}

#endif // SIMULATED_ANNEALING_H