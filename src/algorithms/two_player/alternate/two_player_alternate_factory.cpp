#include "two_player_alternate_factory.h"
#include "random.h"
#include "minimax.h"
#include "alphabeta.h"
#include "deepening.h"
#include "mc.h"
#include "mcts.h"
#include "thunder.h"
#include <stdexcept>

// 두 플레이어 교대 게임 알고리즘 팩토리 구현
std::unique_ptr<Algorithm> TwoPlayerAlternateFactory::createAlgorithm(
    const std::string& algorithmName, const AlgorithmParams& params) {
    
    std::unique_ptr<Algorithm> algorithm;
    
    if (algorithmName == "TwoMazeRandom") {
        algorithm = std::make_unique<TwoMazeRandomAlgorithm>();
    } else if (algorithmName == "Minimax") {
        algorithm = std::make_unique<MinimaxAlgorithm>();
    } else if (algorithmName == "AlphaBeta") {
        algorithm = std::make_unique<AlphaBetaAlgorithm>();
    } else if (algorithmName == "IterativeDeepening") {
        algorithm = std::make_unique<IterativeDeepeningAlgorithm>();
    } else if (algorithmName == "MonteCarlo") {
        algorithm = std::make_unique<MonteCarloAlgorithm>();
    } else if (algorithmName == "MCTS") {
        algorithm = std::make_unique<MCTSAlgorithm>();
    } else if (algorithmName == "Thunder") {
        algorithm = std::make_unique<ThunderAlgorithm>();
    } else if (algorithmName == "ThunderTime") {
        algorithm = std::make_unique<ThunderTimeAlgorithm>();
    } else {
        throw std::invalid_argument("Unknown two player alternate algorithm: " + algorithmName);
    }
    
    algorithm->setParams(params);
    return algorithm;
} 