#include "algorithm_interface.h"
#include "single_player/without_context/single_player_without_context_factory.h"
#include "single_player/with_context/single_player_with_context_factory.h"
#include "two_player/alternate/two_player_alternate_factory.h"
#include "two_player/simultaneous/two_player_simultaneous_factory.h"
#include "connect_four/connect_four_factory.h"
#include <stdexcept>

// 알고리즘 팩토리 구현
std::unique_ptr<Algorithm> AlgorithmFactory::createAlgorithm(
    const std::string& algorithmName, const AlgorithmParams& params) {
    
    // 각 게임 타입에 맞는 알고리즘 이름 접두사를 검사
    
    // 단일 플레이어 컨텍스트 없음 (AutoMaze)
    if (algorithmName == "AutoMazeRandom" || 
        algorithmName == "HillClimb" || 
        algorithmName == "SimulatedAnnealing") {
        return SinglePlayerWithoutContextFactory::createAlgorithm(algorithmName, params);
    }
    
    // 단일 플레이어 컨텍스트 있음 (Maze)
    else if (algorithmName == "MazeRandom" || 
             algorithmName == "Greedy" || 
             algorithmName == "BeamSearch" || 
             algorithmName == "Chokudai") {
        return SinglePlayerWithContextFactory::createAlgorithm(algorithmName, params);
    }
    
    // 두 플레이어 교대 (TwoMaze)
    else if (algorithmName == "TwoMazeRandom" || 
             algorithmName == "Minimax" || 
             algorithmName == "AlphaBeta" || 
             algorithmName == "IterativeDeepening" || 
             algorithmName == "MonteCarlo" || 
             algorithmName == "MCTS" || 
             algorithmName == "Thunder" ||
             algorithmName == "ThunderTime") {
        return TwoPlayerAlternateFactory::createAlgorithm(algorithmName, params);
    }
    
    // 두 플레이어 동시 (SimMaze)
    else if (algorithmName == "SimMazeRandom" || 
             algorithmName == "SimMazeDUCT" || 
             algorithmName == "SimMazePMC" || 
             algorithmName == "SimMazeMCTS") {
        return TwoPlayerSimultaneousFactory::createAlgorithm(algorithmName, params);
    }
    
    // Connect Four
    else if (algorithmName == "ConnectFourRandom" || 
             algorithmName == "ConnectFourMCTS" || 
             algorithmName == "ConnectFourBitMCTS") {
        return ConnectFourFactory::createAlgorithm(algorithmName, params);
    }
    
    else {
        throw std::invalid_argument("Unknown algorithm: " + algorithmName);
    }
}