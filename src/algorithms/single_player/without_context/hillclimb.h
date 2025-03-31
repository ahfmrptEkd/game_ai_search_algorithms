#ifndef HILLCLIMB_ALGORITHM_H
#define HILLCLIMB_ALGORITHM_H

#include "../../../games/automaze/automaze_state.h"

AutoMazeState hillClimbPlacement(const AutoMazeState &state, int number);

void playGame(const std::string& ai_name, AutoMazeState (*ai_func)(const AutoMazeState &, int), const int seed);

inline void playGameHillClimb(const int seed) {
    playGame("HillClimbPlacement", hillClimbPlacement, seed);
}

#endif // HILLCLIMB_ALGORITHM_H