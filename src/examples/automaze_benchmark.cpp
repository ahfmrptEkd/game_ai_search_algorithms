#include "../algorithms/algorithm_interface.h"
#include "../games/automaze/automaze_state.h"
#include "../common/coord.h"
#include "../common/game_util.h"
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <functional>

// 알고리즘 성능 테스트 함수
void testAlgorithmScore(const std::string& name, const std::string& algorithm_name, 
                         const int game_number, const int simulation_count) {
    // 알고리즘 파라미터 설정
    AlgorithmParams params;
    params.searchNumber = simulation_count;

    // 알고리즘 설정(선택)
    if (algorithm_name == "SimulatedAnnealing") {
        params.startTemperature = 1000;
        params.endTemperature = 50;
    }
    
    // 알고리즘 인스턴스 생성
    auto algorithm = AlgorithmFactory::createAlgorithm(algorithm_name, params);
    
    GameUtil::mt_for_action.seed(0);
    double score_mean = 0;
    
    for (int i = 0; i < game_number; i++) {
        auto state = std::make_unique<AutoMazeState>(GameUtil::mt_for_action());
        
        auto result_state = algorithm->runAndEvaluate(*state, 0);
        
        auto score = static_cast<AutoMazeState*>(result_state.get())->getScore();
        score_mean += score;
    }
    
    score_mean /= static_cast<double>(game_number);
    std::cout << name << " Algorithm average score:\t" << score_mean << std::endl;
}

int main(int argc, char* argv[]) {
    std::map<std::string, std::string> algorithms = {
        {"random", "AutoMazeRandom"},
        {"hillclimb", "HillClimb"},
        {"annealing", "SimulatedAnnealing"}
    };
    
    int test_count = 100;          // 알고리즘별 테스트 횟수
    int simulation_count = 1000;  // 시뮬레이션 반복 횟수
    std::string target_algorithm = "";
    
    // 명령줄 인자 처리
    if (argc > 1) {
        target_algorithm = argv[1];
    }
    
    if (argc > 2) {
        test_count = std::stoi(argv[2]);
    }
    
    if (argc > 3) {
        simulation_count = std::stoi(argv[3]);
    }
    
    bool found = false;
    for (const auto& algo_pair : algorithms) {
        if (target_algorithm.empty() || target_algorithm == algo_pair.first) {
            std::cout << algo_pair.first << " algorithm is being tested " << test_count << " times... (simulation) " 
                     << simulation_count << " times)\n";
            testAlgorithmScore(algo_pair.first, algo_pair.second, test_count, simulation_count);
            found = true;
        }
    }
    
    if (!found && !target_algorithm.empty()) {
        std::cout << "Unknown algorithm: " << target_algorithm << "\n";
        std::cout << "Available algorithms:";
        for (const auto& algo_pair : algorithms) {
            std::cout << " " << algo_pair.first;
        }
        std::cout << "\n";
        std::cout << "Usage: ./automaze_benchmark [algorithm] [test count] [simulation count]\n";
        return 1;
    }
    
    return 0;
}