#include "../games/simmaze/simmaze_state.h"
#include "../common/coord.h"
#include "../common/game_util.h"
#include "../algorithms/algorithm_interface.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>

static const std::string direction_strings[4] = {"RIGHT", "LEFT", "DOWN", "UP"};

void playGameWithAlgorithms(
    const std::string& algo1_name, 
    const std::string& algo2_name, 
    const int seed = 0
) {
    
    // 알고리즘 파라미터 설정 (선택)
    AlgorithmParams params1, params2;
    params1.timeThreshold = 100; // 100ms
    params1.playoutNumber = 1000;
    params1.playerId = 0;

    params2.timeThreshold = 100; // 100ms
    params2.playoutNumber = 1000;
    params2.playerId = 1;

    auto algo1 = AlgorithmFactory::createAlgorithm(algo1_name, params1);
    auto algo2 = AlgorithmFactory::createAlgorithm(algo2_name, params2);
    
    std::cout << "Game Start: " << algo1->getName() << " vs " << algo2->getName() << std::endl;

    auto state = std::make_unique<SimMazeState>(seed);

    std::cout << state->toString() << std::endl;
    
    while (!state->isDone()) {
        int action0 = algo1->selectAction(*state);
        int action1 = algo2->selectAction(*state);

        std::cout << "actions: " 
                  << "Player 0: " << direction_strings[action0] << " "
                  << "Player 1: " << direction_strings[action1] << std::endl;
        
        state->advance(action0, action1);

        std::cout << state->toString() << std::endl;
    }

    std::cout << "Game End!" << std::endl;

    auto status = state->getWinningStatus();
    printGameResult(status);
}

void printHelp(const std::map<std::string, std::string>& algorithms) {
    std::cout << "Usage: simmaze_demo [algorithm1] [algorithm2] [seed]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --help                 : print help message" << std::endl;
    std::cout << "  algorithm1, algorithm2 : algorithm name (default: random)" << std::endl;
    std::cout << "  seed                   : seed value for game initialization (default: random)" << std::endl;
    std::cout << std::endl;
    std::cout << "available algorithm list:" << std::endl;
    for (const auto& algo : algorithms) {
        std::cout << "  " << algo.first << std::endl;
    }
}

int main(int argc, char* argv[]) {
    GameUtil::mt_for_action.seed(0);

    // 사용가능한 알고리즘 목록
    std::map<std::string, std::string> algorithms = {
        {"random", "SimMazeRandom"},
        {"duct", "SimMazeDUCT"},
        {"pmc", "SimMazePMC"},
        {"mcts", "SimMazeMCTS"},
    };

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            printHelp(algorithms);
            return 0;
        }
    }

    std::string algo1 = "random";
    std::string algo2 = "random";
    
    if (argc > 1) {
        algo1 = argv[1];
    }
    
    if (argc > 2) {
        algo2 = argv[2];
    }
    
    int seed = GameUtil::mt_for_action();
    if (argc > 3) {
        seed = std::stoi(argv[3]);
    }
    
    
    if (algorithms.find(algo1) == algorithms.end()) {
        std::cerr << "error: algo 1 unknown algorithm - " << algo1 << std::endl;
        printHelp(algorithms);
        return 1;
    }
    
    if (algorithms.find(algo2) == algorithms.end()) {
        std::cerr << "error: algo 2 unknown algorithm - " << algo2 << std::endl;
        printHelp(algorithms);
        return 1;
    }

    std::cout << "Running SimMaze with " << algo1 << " and " << algo2 << " algorithms..." << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    playGameWithAlgorithms(algorithms[algo1], algorithms[algo2], seed);

    return 0;
}