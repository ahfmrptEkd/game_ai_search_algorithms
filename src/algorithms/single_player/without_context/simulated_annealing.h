#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include "../../../games/automaze/automaze_state.h"
#include "../../../common/game_util.h"

AutoMazeState simulatedAnnealingPlacement(const AutoMazeState &state, int number, double start_temp, double end_temp);

#endif // SIMULATED_ANNEALING_H