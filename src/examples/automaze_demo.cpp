#include "../algorithms/algorithm_interface.h"
#include "../games/automaze/automaze_state.h"
#include "../common/coord.h"
#include "../common/game_util.h"
#include "../common/game_constants.h"
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <functional>

void playGameWithAlgorithm(const std::string& algorithm_name, int seed) {
    AlgorithmParams params;
    
        // 게임 설정
    params.searchWidth = 3;
    params.searchDepth = 7;

    if (algorithm_name == "HillClimb" || algorithm_name == "SimulatedAnnealing") {
        params.searchNumber = GameConstants::AlgorithmParams::SEARCH_NUMBER; // 1000번 반복
    }
    
    auto algorithm = AlgorithmFactory::createAlgorithm(algorithm_name, params);
    
    auto state = std::make_unique<AutoMazeState>(seed);
    
    auto result_state = algorithm->runAndEvaluate(*state, 0);
    
    std::cout << result_state->toString() << std::endl;
    
    auto final_score = static_cast<AutoMazeState*>(result_state.get())->getScore(true);
    std::cout << "Final score: " << final_score << std::endl;
}

int main(int argc, char* argv[]) {
    GameUtil::mt_for_action.seed(0);
    
    std::map<std::string, std::string> algorithms = {
        {"random", "AutoMazeRandom"},
        {"hillclimb", "HillClimb"},
        {"annealing", "SimulatedAnnealing"}
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