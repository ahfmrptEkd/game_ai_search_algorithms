#include "../games/simmaze/simmaze_state.h"
#include "../common/coord.h"
#include "../common/game_util.h"
#include "../algorithms/two_player/simultaneous/random.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <functional>

void playGameWithAlgorithm(const std::string& algo1_name, const std::string& algo2_name, const int seed) {
    int simulation_number = 1000;

    std::function<int(const SimMazeState&, const int)> algo1;
    std::function<int(const SimMazeState&, const int)> algo2;

    if (algo1_name == "random") {
        algo1 = simMazeRandomAction;
        std::cout << "algo1: random" << std::endl;
    }
    else {
        std::cout << "Unknown algorithm: " << algo1_name << ". Using Random instead." << std::endl;
        algo1 = simMazeRandomAction;
    }

    if (algo2_name == "random") {
        algo2 = simMazeRandomAction;
        std::cout << "algo2: random" << std::endl;
    }
    else {
        std::cout << "Unknown algorithm: " << algo2_name << ". Using Random instead." << std::endl;
        algo2 = simMazeRandomAction;
    }

    playSimMazeGame(algo1, algo2, seed); 
}

int main(int argc, char* argv[]) {
    GameUtil::mt_for_action.seed(0);

    // 사용가능한 알고리즘 목록
    std::vector<std::string> algorithms = {
        "random",
    };

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
    
    std::cout << "Running SimMaze with " << algo1 << " and " << algo2 << " algorithms..." << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    playGameWithAlgorithm(algo1, algo2, seed);

    return 0;
}