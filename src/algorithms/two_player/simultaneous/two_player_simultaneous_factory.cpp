#include "two_player_simultaneous_factory.h"
#include "random.h"
#include "pmc.h"
#include "mcts_sim.h"
#include "duct.h"
#include <stdexcept>

// 두 플레이어 동시 게임 알고리즘 팩토리 구현
std::unique_ptr<Algorithm> TwoPlayerSimultaneousFactory::createAlgorithm(
    const std::string& algorithmName, const AlgorithmParams& params) {
    
    std::unique_ptr<Algorithm> algorithm;
    
    if (algorithmName == "SimMazeRandom") {
        algorithm = std::make_unique<SimMazeRandomAlgorithm>();
    } else if (algorithmName == "SimMazeDUCT") {
        algorithm = std::make_unique<DuctAlgorithm>();
    } else if (algorithmName == "SimMazePMC") {
        algorithm = std::make_unique<PMCAlgorithm>();
    } else if (algorithmName == "SimMazeMCTS") {
        algorithm = std::make_unique<MCTSSimAlgorithm>();
    } else {
        throw std::invalid_argument("Unknown two player simultaneous algorithm: " + algorithmName);
    }
    
    algorithm->setParams(params);
    return algorithm;
} 