#include "../algorithms/algorithm_interface.h"
#include "../games/twomaze/twomaze_state.h"
#include "../common/coord.h"
#include "../common/game_util.h"
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <functional>

// 알고리즘을 사용해 TwoMaze 게임을 플레이하는 함수
void playGameWithAlgorithm(const std::string& algorithm_name, int seed) {
    AlgorithmParams params;

    // 게임 설정
    params.searchWidth = GameConstants::AlgorithmParams::SEARCH_WIDTH; 
    params.searchDepth = GameConstants::AlgorithmParams::SEARCH_DEPTH; 
    
    // 알고리즘별 파라미터 조정
    if (algorithm_name == "Minimax" || algorithm_name == "AlphaBeta") {
        params.searchDepth = 4;
    } else if (algorithm_name == "IterativeDeepening") {
        params.timeThreshold = 100; // 100ms
    } else if (algorithm_name == "MonteCarlo" || algorithm_name == "MCTS" || algorithm_name == "Thunder") {
        params.playoutNumber = 1000;
    } else if (algorithm_name == "ThunderTime") {
        params.timeThreshold = 100; // 100ms
    }
    
    auto algorithm = AlgorithmFactory::createAlgorithm(algorithm_name, params);
    
    auto state = std::make_unique<TwoMazeState>(seed);
    
    std::cout << state->toString() << std::endl;
    
    while (!state->isDone()) {
        int action = algorithm->selectAction(*state);
        state->progress(action);
        std::cout << state->toString() << std::endl;
    }
    
    WinningStatus status = state->getWinningStatus();
    printGameResult(status);
}

int main(int argc, char* argv[]) {
    GameUtil::mt_for_action.seed(0);
    
    std::map<std::string, std::string> algorithms = {
        {"random", "TwoMazeRandom"},
        {"minimax", "Minimax"},
        {"alphabeta", "AlphaBeta"},
        {"deepening", "IterativeDeepening"},
        {"mc", "MonteCarlo"},
        {"mcts", "MCTS"},
        {"thunder", "Thunder"},
        {"thunder_time", "ThunderTime"}
    };
    
    std::string algorithm = "random";
    
    if (argc > 1) {
        algorithm = argv[1];
    }
    
    auto it = algorithms.find(algorithm);
    if (it != algorithms.end()) {
        std::cout << "Running " << algorithm << " algorithm...\n";
        playGameWithAlgorithm(it->second, GameUtil::mt_for_action());
    } else {
        std::cout << "Unknown algorithm: " << algorithm << "\n";
        std::cout << "Available algorithms:";
        for (const auto& pair : algorithms) {
            std::cout << " " << pair.first;
        }
        std::cout << "\n";
        return 1;
    }
    
    return 0;
}