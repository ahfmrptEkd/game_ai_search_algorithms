#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include "../../../games/automaze/automaze_state.h"
#include "../../../common/game_util.h"

AutoMazeState simulatedAnnealingPlacement(const AutoMazeState &state, int number, double start_temp, double end_temp);

void playGame(const std::string& ai_name, AutoMazeState (*ai_func)(const AutoMazeState &, int, double, double), const int seed);

inline void playGameSimulatedAnnealing(const int seed) {
    playGame("SimulatedAnnealingPlacement", simulatedAnnealingPlacement, seed);
}

#endif // SIMULATED_ANNEALING_H