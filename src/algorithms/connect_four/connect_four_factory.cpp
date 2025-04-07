#include "connect_four_factory.h"
#include "connect_four_algorithms.h" // Connect Four 알고리즘들이 정의된 헤더
#include <stdexcept>

// Connect Four 게임 알고리즘 팩토리 구현
std::unique_ptr<Algorithm> ConnectFourFactory::createAlgorithm(
    const std::string& algorithmName, const AlgorithmParams& params) {
    
    std::unique_ptr<Algorithm> algorithm;
    
    if (algorithmName == "ConnectFourRandom") {
        algorithm = std::make_unique<ConnectFourRandomAlgorithm>();
    } else if (algorithmName == "ConnectFourMCTS") {
        algorithm = std::make_unique<ConnectFourMCTSAlgorithm>();
    } else if (algorithmName == "ConnectFourBitMCTS") {
        algorithm = std::make_unique<ConnectFourBitMCTSAlgorithm>();
    } else {
        throw std::invalid_argument("Unknown Connect Four algorithm: " + algorithmName);
    }
    
    algorithm->setParams(params);
    return algorithm;
} 