#include "single_player_with_context_factory.h"
#include "random.h"
#include "greedy.h"
#include "beam.h"
#include "chokudai.h"
#include <stdexcept>

// 단일 플레이어 컨텍스트 있는 알고리즘 팩토리 구현
std::unique_ptr<Algorithm> SinglePlayerWithContextFactory::createAlgorithm(
    const std::string& algorithmName, const AlgorithmParams& params) {
    
    std::unique_ptr<Algorithm> algorithm;
    
    if (algorithmName == "MazeRandom") {
        algorithm = std::make_unique<MazeRandomAlgorithm>();
    } else if (algorithmName == "Greedy") {
        algorithm = std::make_unique<GreedyAlgorithm>();
    } else if (algorithmName == "BeamSearch") {
        algorithm = std::make_unique<BeamSearchAlgorithm>();
    } else if (algorithmName == "Chokudai") {
        algorithm = std::make_unique<ChokudaiAlgorithm>();
    } else {
        throw std::invalid_argument("Unknown single player with context algorithm: " + algorithmName);
    }
    
    algorithm->setParams(params);
    return algorithm;
} 