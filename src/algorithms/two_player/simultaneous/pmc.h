#ifndef SIM_PMC_H
#define SIM_PMC_H

#include "../../../games/simmaze/simmaze_state.h"
#include "../../../common/game_util.h"
#include "random.h"

namespace sim_pmc {
    double playout(SimMazeState* state);
    
    int primitiveMontecarloAction(const SimMazeState& state, const int player_id, const int playout_number);
}

int pmcSearchAction(const SimMazeState& state, const int player_id, int simulation_number);

inline void playSimMazeGamePMC(const int seed, int simulation_number = 1000) {
    playSimMazeGame(
        [simulation_number](const SimMazeState& state, const int player_id) {
            return pmcSearchAction(state, player_id, simulation_number);
        },
        [simulation_number](const SimMazeState& state, const int player_id) {
            return pmcSearchAction(state, player_id, simulation_number);
        },
        seed);
}

#endif // SIM_PMC_H