#pragma once

#include "../algorithm_interface.h"
#include "../../games/connect_four/connect_four_state.h"
#include "../../games/connect_four/connect_four_bitboard.h"
#include <memory>
#include <random>
#include <chrono>
#include <limits>
#include <algorithm>

class ConnectFourBitIterativeDeepeningAlgorithm : public Algorithm {
private:
    AlgorithmParams params_;
    std::mt19937 random_engine_;
    
    // 알파베타 프루닝을 사용한 미니맥스 알고리즘 (비트보드 버전)
    std::pair<int, double> alphabeta(const ConnectFourBitBoardState& state, int depth, double alpha, double beta, bool maximizing_player);
    
    // Iterative Deepening 구현 (비트보드 버전)
    int iterativeDeepening(const ConnectFourState& state);
    
public:
    ConnectFourBitIterativeDeepeningAlgorithm();
    int selectAction(const GameState& state) override;
    std::string getName() const override;
    void setParams(const AlgorithmParams& params) override;
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override;
}; 