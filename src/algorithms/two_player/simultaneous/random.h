#ifndef SIM_RANDOM_H
#define SIM_RANDOM_H

#include "../../../games/simmaze/simmaze_state.h"
#include <functional>

int simMazeRandomAction(const SimMazeState& state, const int player_id);

#endif // SIM_RANDOM_H