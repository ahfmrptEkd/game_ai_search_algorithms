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
    
    int thunderSearchAction(const TwoMazeState& state, int simulation_number);

    int thunderSearchActionWithTime(const TwoMazeState& state, const int64_t time_ms);
}

int thunderSearchAction(const TwoMazeState& state, int simulation_number);

int thunderSearchActionWithTime(const TwoMazeState& state, const int64_t time_ms);

inline void playGameThunder(const int seed, int simulation_number = 1000) {
    playGame([simulation_number](const TwoMazeState& state) {
        return thunderSearchAction(state, simulation_number);
    }, seed);
}

inline void playGameThunderWithTime(const int seed, const int64_t time_ms) {
    playGame([time_ms](const TwoMazeState& state) {
        return thunderSearchActionWithTime(state, time_ms);
    }, seed);
}

#endif // THUNDER_H