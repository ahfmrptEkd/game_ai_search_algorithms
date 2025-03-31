#include "../algorithms/single_player/without_context/random.h"
#include "../algorithms/single_player/without_context/hillclimb.h"
#include "../algorithms/single_player/without_context/simulated_annealing.h"
#include "../common/coord.h"
#include "../common/game_util.h"
#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <random>

// 알고리즘 성능 테스트 함수
void testAlgorithmScore(const std::string& name, 
                         std::function<AutoMazeState(const AutoMazeState&, int)> placement_func,
                         const int game_number, 
                         const int simulation_count)
{
    GameUtil::mt_for_action.seed(0);
    double score_mean = 0;
    
    for (int i = 0; i < game_number; i++)
    {
        auto state = AutoMazeState(GameUtil::mt_for_action());
        state = placement_func(state, simulation_count);    // 알고리즘으로 캐릭터 배치
        auto score = state.getScore();
        score_mean += score;
    }
    
    score_mean /= static_cast<double>(game_number);
    std::cout << name << " 알고리즘 평균 점수:\t" << score_mean << std::endl;
}

// 모의 담금질 알고리즘 어댑터 함수
AutoMazeState simulatedAnnealingAdapter(const AutoMazeState& state, int count) {
    return simulatedAnnealingPlacement(state, count, 500.0, 10.0);
}

int main(int argc, char* argv[]) 
{
    // 알고리즘 맵 구성
    struct AlgorithmInfo {
        std::string name;
        std::function<AutoMazeState(const AutoMazeState&, int)> placement_func;
    };
    
    std::vector<AlgorithmInfo> algorithms = {
        {"random", [](const AutoMazeState& state, int count) { return randomPlacement(state); }},
        {"hillclimb", hillClimbPlacement},
        {"annealing", simulatedAnnealingAdapter}
    };
    
    int test_count = 100;          // 알고리즘별 테스트 횟수
    int simulation_count = 10000;  // 시뮬레이션 반복 횟수
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
    for (const auto& algo : algorithms) {
        if (target_algorithm.empty() || target_algorithm == algo.name) {
            std::cout << algo.name << " 알고리즘을 " << test_count << "회 테스트 중... (시뮬레이션 " << simulation_count << "회)\n";
            testAlgorithmScore(algo.name, algo.placement_func, test_count, simulation_count);
            found = true;
        }
    }
    
    if (!found && !target_algorithm.empty()) {
        std::cout << "알 수 없는 알고리즘: " << target_algorithm << "\n";
        std::cout << "사용 가능한 알고리즘:";
        for (const auto& algo : algorithms) {
            std::cout << " " << algo.name;
        }
        std::cout << "\n";
        std::cout << "사용법: ./automaze_benchmark [알고리즘] [테스트횟수] [시뮬레이션횟수]\n";
        return 1;
    }
    
    return 0;
}