#include "../algorithms/algorithm_interface.h"
#include "../games/maze/maze_state.h"
#include "../common/coord.h"
#include "../common/game_util.h"
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <functional>

// 알고리즘 성능 테스트 함수
void testAlgorithmScore(const int game_number, const std::string& algorithm_name, int64_t time_ms) {
    AlgorithmParams params;
    
    if (algorithm_name == "BeamSearch" || algorithm_name == "Chokudai") {
        params.timeThreshold = time_ms; // 1ms 시간 제한
    }
    
    // 알고리즘 인스턴스 생성
    auto algorithm = AlgorithmFactory::createAlgorithm(algorithm_name, params);
    
    GameUtil::mt_for_action.seed(0);
    double score_mean = 0;
    
    for (int i = 0; i < game_number; i++) {
        auto state = std::make_unique<MazeState>(GameUtil::mt_for_action());
        
        while (!state->isDone()) {
            int action = algorithm->selectAction(*state);
            state->progress(action);
        }
        
        auto score = static_cast<MazeState*>(state.get())->game_score_;
        score_mean += score;
    }
    
    score_mean /= static_cast<double>(game_number);
    std::cout << "평균 점수:\t" << score_mean << std::endl;
}

int main(int argc, char* argv[]) {
    // 사용 가능한 알고리즘 목록
    std::map<std::string, std::string> algorithms = {
        {"random", "MazeRandom"},
        {"greedy", "Greedy"},
        {"beam", "BeamSearch"},
        {"chokudai", "Chokudai"}
    };
    
    std::string algorithm = "random";
    int test_count = 100; // 기본 테스트 횟수
    int64_t time_ms = 1; // 기본 시간 제한
    
    if (argc > 1) {
        algorithm = argv[1];
    }
    
    if (argc > 2) {
        test_count = std::stoi(argv[2]);
    }

    if (argc > 3) {
        time_ms = std::stoi(argv[3]);
    }
    
    auto it = algorithms.find(algorithm);
    if (it != algorithms.end()) {
        std::cout << algorithm << " 알고리즘을 " << test_count << "회 테스트 중...\n";
        testAlgorithmScore(test_count, it->second, time_ms);
    } else {
        std::cout << "알 수 없는 알고리즘: " << algorithm << "\n";
        std::cout << "사용 가능한 알고리즘:";
        for (const auto& pair : algorithms) {
            std::cout << " " << pair.first;
        }
        std::cout << "\n";
        std::cout << "사용법: ./maze_benchmark [알고리즘] [테스트횟수]\n";
        return 1;
    }
    
    return 0;
}