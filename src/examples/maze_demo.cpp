#include "../algorithms/algorithm_interface.h"
#include "../games/maze/maze_state.h"
#include "../common/coord.h"
#include "../common/game_util.h"
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <functional>

void playGameWithAlgorithm(const std::string& algorithm_name, int seed) {
    AlgorithmParams params;
    
    // 게임 설정 (선택)
    params.searchWidth = GameConstants::AlgorithmParams::SEARCH_WIDTH;   // 3
    params.searchDepth = GameConstants::AlgorithmParams::SEARCH_DEPTH;   // 7
    params.searchNumber = 100;

    if (algorithm_name == "BeamSearch" || algorithm_name == "Chokudai") {
        params.timeThreshold = 1; // 1ms 시간 제한
    }
    
    auto algorithm = AlgorithmFactory::createAlgorithm(algorithm_name, params);
    
    auto state = std::make_unique<MazeState>(seed);
    
    std::cout << state->toString() << std::endl;
    
    while (!state->isDone()) {
        int action = algorithm->selectAction(*state);
        state->progress(action);
        std::cout << state->toString() << std::endl;
    }
    
    std::cout << "Final score: " << static_cast<MazeState*>(state.get())->game_score_ << std::endl;
}

int main(int argc, char* argv[]) {
    GameUtil::mt_for_action.seed(0);
    
    std::map<std::string, std::string> algorithms = {
        {"random", "MazeRandom"},
        {"greedy", "Greedy"},          
        {"beam", "BeamSearch"},      
        {"chokudai", "Chokudai"}  
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