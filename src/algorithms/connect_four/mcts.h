#pragma once

#include "../algorithm_interface.h"
#include "../../games/connect_four/connect_four_state.h"
#include "../../common/game_util.h"
#include <memory>
#include <vector>
#include <cmath>
#include <chrono>

class ConnectFourMCTSAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
    // MCTS 구현에 필요한 상수 및 유틸리티
    static constexpr const double C = 1.;
    static constexpr const int EXPAND_THRESHOLD = 10;
    
    static double playout(ConnectFourState* state);
    
    // MCTS 노드 클래스
    class Node {
    private:
        ConnectFourState state_;
        double w_;
        
    public:
        std::vector<Node> child_nodes_;
        double n_;
        
        Node(const ConnectFourState& state);
        
        double evaluate();
        void expand();
        Node& nextChildNode();
    };
    
public:
    int selectAction(const GameState& state) override;
    std::string getName() const override;
    void setParams(const AlgorithmParams& params) override;
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override;
}; 