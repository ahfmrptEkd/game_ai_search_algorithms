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

#endif // SIM_PMC_H