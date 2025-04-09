#ifndef DUCT_H
#define DUCT_H

#include "../../../games/simmaze/simmaze_state.h"
#include "../../../common/game_util.h"
#include "random.h"
#include <vector>
#include <memory>

namespace sim_duct {
    constexpr const double C = 1.0; // UCB1 계산에 사용하는 상수
    constexpr const int EXPAND_THRESHOLD = 5; // 노드 확장 임계값
    
    double playout(SimMazeState* state);
    
    class Node {
    private:
        SimMazeState state_;
        double w_;
        
    public:
        std::vector<std::vector<Node>> child_nodeses_; // [action0][action1] 형태의 자식 노드 배열
        double n_;
        
        Node() : w_(0), n_(0) {}
        
        Node(const SimMazeState& state);
        
        double evaluate();
        
        void expand();
        
        Node& nextChildNode();
    };
    
    int ductAction(const SimMazeState& state, const int player_id, const int simulation_number);
}

int ductSearchAction(const SimMazeState& state, const int player_id, int simulation_number);

#endif // DUCT_H