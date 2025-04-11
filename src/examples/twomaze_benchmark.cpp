#include "../algorithms/algorithm_interface.h"
#include "../games/twomaze/twomaze_state.h"
#include "../common/coord.h"
#include "../common/game_util.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <chrono>
#include <ctime>

struct BenchmarkResult {
    double win_rate;
    double avg_score;
    double avg_time_ms;
};

// 알고리즘 파라미터 구성 유틸리티 함수
void configureAlgorithmParams(const std::string& algo_name, AlgorithmParams& params, 
                             int simulation_count, int64_t time_threshold) {
    if (algo_name == "Minimax" || algo_name == "AlphaBeta") {
        params.searchDepth = GameConstants::AlgorithmParams::SEARCH_DEPTH;
    } else if (algo_name == "IterativeDeepening") {
        params.timeThreshold = time_threshold;
    } else if (algo_name == "MonteCarlo" || algo_name == "MCTS" || algo_name == "Thunder") {
        params.playoutNumber = simulation_count;
    } else if (algo_name == "ThunderTime") {
        params.timeThreshold = time_threshold;
    }
}

BenchmarkResult testAlgorithmPerformance(
    const std::string& test_algo_name,
    const std::string& opponent_algo_name,
    int game_count,
    int simulation_count,
    int64_t time_threshold
) {
    // 테스트 알고리즘 설정
    AlgorithmParams test_params;
    configureAlgorithmParams(test_algo_name, test_params, simulation_count, time_threshold);
    auto test_algo = AlgorithmFactory::createAlgorithm(test_algo_name, test_params);
    
    // 상대 알고리즘 설정
    AlgorithmParams opponent_params;
    configureAlgorithmParams(opponent_algo_name, opponent_params, simulation_count, time_threshold);
    auto opponent_algo = AlgorithmFactory::createAlgorithm(opponent_algo_name, opponent_params);
    
    double win_count = 0;
    double score_sum = 0;
    double total_time_ms = 0;
    
    for (int i = 0; i < game_count; i++) {
        
        // 선공과 후공을 번갈아가며 테스트 (공정성 확보)
        for (int first_player = 0; first_player < 2; first_player++) {
            auto state = std::make_unique<TwoMazeState>(i);
            
            // 현재 플레이어가 테스트 알고리즘인지 상대 알고리즘인지 결정
            auto& player1 = (first_player == 0) ? test_algo : opponent_algo;
            auto& player2 = (first_player == 0) ? opponent_algo : test_algo;
            
            while (!state->isDone()) {
                // 시간 측정은 테스트 알고리즘의 턴일 때만
                if ((state->getCurrentTurn() % 2 == 0 && first_player == 0) || 
                    (state->getCurrentTurn() % 2 == 1 && first_player == 1)) {
                    auto start_time = std::chrono::high_resolution_clock::now();
                    int action = player1->selectAction(*state);
                    auto end_time = std::chrono::high_resolution_clock::now();
                    
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                        end_time - start_time).count();
                    total_time_ms += duration;
                    
                    state->progress(action);
                } else {
                    state->progress(player2->selectAction(*state));
                }
            }
            
            WinningStatus status = state->getWinningStatus();
            bool is_win = (first_player == 0 && status == WinningStatus::WIN) ||
                         (first_player == 1 && status == WinningStatus::LOSE);
            bool is_draw = status == WinningStatus::DRAW;
            
            if (is_win) win_count += 1.0;
            else if (is_draw) win_count += 0.5;
            
            score_sum += state->getCurrentPlayerScore();
        }
    }
    
    BenchmarkResult result;
    // 총 게임 수는 game_count * 2 (선공/후공 각각)
    result.win_rate = win_count / (game_count * 2);
    result.avg_score = score_sum / (game_count * 2);
    result.avg_time_ms = total_time_ms / (game_count * 2);
    
    return result;
}

// 모든 알고리즘 쌍에 대해 대결 시키는 함수
void runFullBenchmark(const std::map<std::string, std::string>& algorithms, 
                      int game_count, int simulation_count, int64_t time_threshold) {
    std::cout << "Running full benchmark... " << game_count << " games/algorithm pair" << std::endl;
    
    std::vector<std::string> algo_names;
    std::vector<std::string> algo_internal_names;
    
    for (const auto& algo_pair : algorithms) {
        algo_names.push_back(algo_pair.first);
        algo_internal_names.push_back(algo_pair.second);
    }
    
    // 헤더 출력 - 각 알고리즘 이름 사이에 충분한 간격
    std::cout << std::left << std::setw(14) << "Algorithm";
    for (const auto& name : algo_names) {
        std::cout << std::setw(14) << name;
    }
    std::cout << std::setw(12) << "Average win rate (%)" << std::endl;
    
    // 각 알고리즘에 대한 승률 표 생성
    for (size_t i = 0; i < algo_names.size(); i++) {
        std::cout << std::left << std::setw(14) << algo_names[i];
        
        double total_win_rate = 0.0;
        int opponent_count = 0;
        
        for (size_t j = 0; j < algo_names.size(); j++) {
            if (i == j) {
                std::cout << std::setw(14) << "-";
                continue;
            }
            
            BenchmarkResult result = testAlgorithmPerformance(
                algo_internal_names[i],
                algo_internal_names[j],
                game_count,
                simulation_count,
                time_threshold
            );
            
            std::cout << std::right << std::setw(10) << std::fixed << std::setprecision(2) 
                      << (result.win_rate * 100) << "%   ";
            
            total_win_rate += result.win_rate;
            opponent_count++;
        }
        
        // 평균 승률 계산 및 출력
        if (opponent_count > 0) {
            double avg_win_rate = total_win_rate / opponent_count;
            std::cout << std::right << std::setw(10) << std::fixed << std::setprecision(2) 
                      << (avg_win_rate * 100) << "%";
        } else {
            std::cout << std::setw(12) << "-";
        }
        std::cout << std::endl;
    }
}

// 시간 제한 기반으로 알고리즘 성능 분석하는 함수
void analyzeTimeConstraints(const std::map<std::string, std::string>& time_algorithms,
                           const std::string& baseline_algo_name,
                           const std::vector<int64_t>& time_limits,
                           int game_count,
                           int simulation_count) {
    std::cout << "\nTime-based performance analysis..." << std::endl;
    
    std::cout << std::setw(20) << "Algorithm";
    for (const auto& time_ms : time_limits) {
        std::cout << std::setw(10) << time_ms << "ms";
    }
    std::cout << std::endl;
    
    for (const auto& algo_pair : time_algorithms) {
        std::cout << std::setw(20) << algo_pair.first;
        
        for (const auto& time_ms : time_limits) {
            
            // 기준 알고리즘과 대결
            BenchmarkResult result = testAlgorithmPerformance(
                algo_pair.second,
                baseline_algo_name,
                game_count,
                simulation_count,
                time_ms  // 이 알고리즘의 시간 제한 설정
            );
            
            std::cout << std::setw(10) << std::fixed << std::setprecision(2) 
                      << (result.win_rate * 100) << "%";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    GameUtil::mt_for_action.seed(time(nullptr));
    
    // 기본 설정값
    int game_count = 100;        // 기본 게임 수
    int simulation_count = GameConstants::AlgorithmParams::SEARCH_NUMBER; // 기본 시뮬레이션 수
    int64_t time_threshold = 10; // 기본 시간 제한 (밀리초)
    std::string benchmark_mode = "all"; // 기본 모드: 모든 알고리즘 비교
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--games" && i + 1 < argc) {
            game_count = std::stoi(argv[++i]);
        } else if (arg == "--sims" && i + 1 < argc) {
            simulation_count = std::stoi(argv[++i]);
        } else if (arg == "--mode" && i + 1 < argc) {
            benchmark_mode = argv[++i];
        } else if (arg == "--help") {
            std::cout << "Usage: twomaze_benchmark [options]\n"
                      << "Options:\n"
                      << "  --games N        Number of games per algorithm pair\n"
                      << "  --sims N         Number of simulations for simulation-based algorithms\n"
                      << "  --mode MODE      Benchmark mode (all, time)\n"
                      << "  --help           Show this help message\n";
            return 0;
        }
    }
    
    // 알고리즘 매핑
    std::map<std::string, std::string> algorithms = {
        {"Random", "TwoMazeRandom"},
        {"Minimax", "Minimax"},
        {"AlphaBeta", "AlphaBeta"},
        {"Deepening", "IterativeDeepening"},
        {"MonteCarlo", "MonteCarlo"},
        {"MCTS", "MCTS"},
        {"Thunder", "Thunder"}
    };
    
    // 시간 제한 테스트용 알고리즘
    std::map<std::string, std::string> time_algorithms = {
        {"MCTS", "MCTS"},
        {"Thunder", "Thunder"},
        {"Deepening", "IterativeDeepening"}
    };
    
    std::vector<int64_t> time_limits = {1, 10, 50, 100, 250, 500, 1000};
    
    try {
        if (benchmark_mode == "all" || benchmark_mode == "full") {
            runFullBenchmark(algorithms, game_count, simulation_count, time_threshold);
        } else if (benchmark_mode == "time") {
            // 기준 알고리즘: MonteCarlo
            analyzeTimeConstraints(time_algorithms, "MonteCarlo", time_limits, game_count, simulation_count);
        } else {
            std::cout << "Unknown mode: " << benchmark_mode << "\n";
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}