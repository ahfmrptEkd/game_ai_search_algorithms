#pragma once

#include "../algorithm_interface.h"
#include "../../games/connect_four/connect_four_state.h"
#include "../../games/connect_four/connect_four_bitboard.h"
#include "../../common/game_util.h"
#include <memory>
#include <vector>
#include <cmath>
#include <chrono>

class ConnectFourBitMCTSAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    
    // 플레이아웃 함수
    static double playout(ConnectFourBitBoardState* state);
    
    // MCTS 노드 클래스 (비트보드 버전)
    class Node {
    private:
        ConnectFourBitBoardState state_;
        double w_;
        
    public:
        std::vector<Node> child_nodes_;
        double n_;
        
        static constexpr const double C = 1.;
        static constexpr const int EXPAND_THRESHOLD = 10;
        
        Node(const ConnectFourBitBoardState& state);
        
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