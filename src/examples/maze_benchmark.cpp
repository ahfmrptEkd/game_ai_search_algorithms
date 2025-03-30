#include "../algorithms/single_player/with_context/random.h"
#include "../algorithms/single_player/with_context/greedy.h"
#include "../algorithms/single_player/with_context/beam.h"
#include "../algorithms/single_player/with_context/chokudai.h"
#include "../common/coord.h"
#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <random>

void testAlgorithmScore(const int game_number, std::function<int(const State&)> strategy_func)
{
    std::mt19937 mt_for_construct(0);
    double score_mean = 0;
    for (int i = 0; i < game_number; i++)
    {
        auto state = State(mt_for_construct());
        while (!state.isDone())
        {
            state.progress(strategy_func(state));
        }
        auto score = state.game_score_;
        score_mean += score;
    }
    score_mean /= static_cast<double>(game_number);
    std::cout << "평균 점수:\t" << score_mean << std::endl;
}

int main(int argc, char* argv[]) 
{
    std::map<std::string, std::function<int(const State&)>> algorithms = 
    {
        {"random", randomAction},
        {"greedy", greedyAction},
        {"beam", [](const State& state){ 
            BeamConfig config; 
            config.time_threshold = 1;
            return beamSearchAction(state, config); 
        }},
        {"chokudai", [](const State& state){ 
            ChokudaiConfig config; 
            config.time_threshold = 1;
            return chokudaiSearchAction(state, config); 
        }}
    };
    
    std::string algorithm = "random";
    int test_count = 100; // 기본 테스트 횟수
    
    if (argc > 1) {
        algorithm = argv[1];
    }
    
    if (argc > 2) {
        test_count = std::stoi(argv[2]);
    }
    
    auto it = algorithms.find(algorithm);
    if (it != algorithms.end()) 
    {
        std::cout << algorithm << " 알고리즘을 " << test_count << "회 테스트 중...\n";
        testAlgorithmScore(test_count, it->second);
    } 
    else 
    {
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