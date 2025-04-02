#ifndef THUNDER_H
#define THUNDER_H

#include "../../../games/twomaze/twomaze_state.h"
#include "../../../common/game_util.h"
#include "mcts.h"
#include "random.h"

namespace thunder {
    class Node {
    private:
        TwoMazeState state_;
        double w_; 
        
    public:
        std::vector<Node> child_nodes_;
        double n_; 
        
        Node(const TwoMazeState& state);
        
        double evaluate();
        
        void expand();
        
        Node& nextChildNode();
    };
    
    int thunderAction(const TwoMazeState& state, int simulation_number);
}

int thunderSearchAction(const TwoMazeState& state, int simulation_number);

inline void playGameThunder(const int seed, int simulation_number = 1000) {
    playGame([simulation_number](const TwoMazeState& state) {
        return thunderSearchAction(state, simulation_number);
    }, seed);
}

#endif // THUNDER_H