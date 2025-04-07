#include "single_player_without_context_factory.h"
#include "random.h"
#include "hillclimb.h"
#include "simulated_annealing.h"
#include <stdexcept>

// 단일 플레이어 컨텍스트 없는 알고리즘 팩토리 구현
std::unique_ptr<Algorithm> SinglePlayerWithoutContextFactory::createAlgorithm(
    const std::string& algorithmName, const AlgorithmParams& params) {
    
    std::unique_ptr<Algorithm> algorithm;
    
    if (algorithmName == "AutoMazeRandom") {
        algorithm = std::make_unique<AutoMazeRandomAlgorithm>();
    } else if (algorithmName == "HillClimb") {
        algorithm = std::make_unique<HillClimbAlgorithm>();
    } else if (algorithmName == "SimulatedAnnealing") {
        algorithm = std::make_unique<SimulatedAnnealingAlgorithm>();
    } else {
        throw std::invalid_argument("Unknown single player without context algorithm: " + algorithmName);
    }
    
    algorithm->setParams(params);
    return algorithm;
} 