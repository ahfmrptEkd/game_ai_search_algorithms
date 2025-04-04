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
    
    // 게임 설정
    params.searchWidth = 5;
    params.searchDepth = 5;
    params.searchNumber = 100;

    if (algorithm_name == "BeamSearch" || algorithm_name == "Chokudai") {
        params.timeThreshold = 1; // 1ms 시간 제한
    }
    
    // 알고리즘 인스턴스 생성
    auto algorithm = AlgorithmFactory::createAlgorithm(algorithm_name, params);
    
    // 게임 상태 초기화
    auto state = std::make_unique<MazeState>(seed);
    
    // 게임 진행
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
    
    // 사용 가능한 알고리즘 목록
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