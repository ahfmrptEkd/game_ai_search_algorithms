#include "../algorithms/two_player/alternate/random.h"
#include "../algorithms/two_player/alternate/minimax.h"
#include "../algorithms/two_player/alternate/alphabeta.h"
#include "../algorithms/two_player/alternate/deepening.h"
#include "../algorithms/two_player/alternate/mc.h"
#include "../algorithms/two_player/alternate/mcts.h"
#include "../algorithms/two_player/alternate/thunder.h"
#include "../common/game_util.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <chrono>
#include <ctime>

struct AlgorithmInfo {
    std::string name;
    std::function<int(const TwoMazeState&)> action_func;
};

struct BenchmarkResult {
    double win_rate;
    double avg_score;
    double avg_simulation_count;
    double avg_time_ms;
};

BenchmarkResult testAlgorithmPerformance(
    const std::function<int(const TwoMazeState&)>& test_algo,
    const std::function<int(const TwoMazeState&)>& opponent_algo,
    int game_count
) {
    double win_count = 0;
    double score_sum = 0;
    double total_time_ms = 0;
    
    for (int i = 0; i < game_count; i++) {
        // 선공과 후공을 번갈아가며 테스트 (공정성 확보)
        for (int first_player = 0; first_player < 2; first_player++) {
            auto state = TwoMazeState(i);
            auto& player1_func = (first_player == 0) ? test_algo : opponent_algo;
            auto& player2_func = (first_player == 0) ? opponent_algo : test_algo;
            
            while (!state.isDone()) {
                // 시간 측정은 테스트 알고리즘의 턴일 때만
                if ((state.getCurrentTurn() % 2 == 0 && first_player == 0) || 
                    (state.getCurrentTurn() % 2 == 1 && first_player == 1)) {
                    auto start_time = std::chrono::high_resolution_clock::now();
                    int action = player1_func(state);
                    auto end_time = std::chrono::high_resolution_clock::now();
                    
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                        end_time - start_time).count();
                    total_time_ms += duration;
                    
                    state.progress(action);
                } else {
                    state.progress(player2_func(state));
                }
            }
            
            WinningStatus status = state.getWinningStatus();
            bool is_win = (first_player == 0 && status == WinningStatus::WIN) ||
                         (first_player == 1 && status == WinningStatus::LOSE);
            bool is_draw = status == WinningStatus::DRAW;
            
            if (is_win) win_count += 1.0;
            else if (is_draw) win_count += 0.5;
            
            int player_index = (first_player == 0) ? 0 : 1;
            score_sum += state.getCurrentPlayerScore();
        }
    }
    
    BenchmarkResult result;
    // 총 게임 수는 game_count * 2 (선공/후공 각각)
    result.win_rate = win_count / (game_count * 2);
    result.avg_score = score_sum / (game_count * 2);
    result.avg_time_ms = total_time_ms / (game_count * 2);
    result.avg_simulation_count = 0; 
    
    return result;
}

// 모든 알고리즘 쌍에 대해 대결 시키는 함수
void runFullBenchmark(const std::vector<AlgorithmInfo>& algorithms, int game_count) {
    std::cout << "전체 벤치마크 실행 중... " << game_count << "게임/알고리즘 쌍\n" << std::endl;
    
    std::cout << std::setw(20) << "알고리즘";
    for (const auto& algo : algorithms) {
        std::cout << std::setw(12) << algo.name;
    }
    std::cout << std::setw(12) << "평균 승률%" << std::endl;
    
    // 각 알고리즘에 대한 승률 표 생성
    for (size_t i = 0; i < algorithms.size(); i++) {
        std::cout << std::setw(20) << algorithms[i].name;
        
        double total_win_rate = 0.0;
        int opponent_count = 0;
        
        for (size_t j = 0; j < algorithms.size(); j++) {
            if (i == j) {
                std::cout << std::setw(12) << "-";
                continue;
            }
            
            BenchmarkResult result = testAlgorithmPerformance(
                algorithms[i].action_func, 
                algorithms[j].action_func, 
                game_count
            );
            
            std::cout << std::setw(12) << std::fixed << std::setprecision(2) 
                      << (result.win_rate * 100) << "%";
            
            total_win_rate += result.win_rate;
            opponent_count++;
        }
        
        // 평균 승률 계산 및 출력
        if (opponent_count > 0) {
            double avg_win_rate = total_win_rate / opponent_count;
            std::cout << std::setw(12) << std::fixed << std::setprecision(2) 
                      << (avg_win_rate * 100) << "%";
        } else {
            std::cout << std::setw(12) << "-";
        }
        std::cout << std::endl;
    }
}

// 시간 제한 기반으로 알고리즘 성능 분석하는 함수
void analyzeTimeConstraints(const std::vector<AlgorithmInfo>& time_algorithms, 
                           const AlgorithmInfo& baseline_algo,
                           const std::vector<int64_t>& time_limits,
                           int game_count) {
    std::cout << "\n시간 제한 기반 성능 분석 중...\n" << std::endl;
    
    std::cout << std::setw(20) << "알고리즘";
    for (const auto& time_ms : time_limits) {
        std::cout << std::setw(10) << time_ms << "ms";
    }
    std::cout << std::endl;
    
    for (const auto& algo_info : time_algorithms) {
        std::cout << std::setw(20) << algo_info.name;
        
        for (const auto& time_ms : time_limits) {
            auto time_constrained_func = algo_info.action_func;
            
            // 기준 알고리즘과 대결
            BenchmarkResult result = testAlgorithmPerformance(
                time_constrained_func,
                baseline_algo.action_func,
                game_count
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
    int game_count = 10;        // 기본 게임 수
    int simulation_count = 1000; // 기본 시뮬레이션 수
    int64_t time_threshold = 100; // 기본 시간 제한 (밀리초)
    std::string benchmark_mode = "all"; // 기본 모드: 모든 알고리즘 비교
    std::string algo1 = "";     // 상세 비교용 알고리즘 1
    std::string algo2 = "";     // 상세 비교용 알고리즘 2
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--games" && i + 1 < argc) {
            game_count = std::stoi(argv[++i]);
        } else if (arg == "--sims" && i + 1 < argc) {
            simulation_count = std::stoi(argv[++i]);
        } else if (arg == "--time" && i + 1 < argc) {
            time_threshold = std::stol(argv[++i]);
        } else if (arg == "--mode" && i + 1 < argc) {
            benchmark_mode = argv[++i];
        } else if (arg == "--algo1" && i + 1 < argc) {
            algo1 = argv[++i];
        } else if (arg == "--algo2" && i + 1 < argc) {
            algo2 = argv[++i];
        } else if (arg == "--help") {
            std::cout << "사용법: twomaze_benchmark [옵션]\n"
                      << "옵션:\n"
                      << "  --games N        Number of games per algorithm pair\n"
                      << "  --sims N         Number of simulations for simulation-based algorithms\n"
                      << "  --time N         Time threshold in milliseconds for time-based algorithms\n"
                      << "  --mode MODE      Benchmark mode (all, time, detail)\n"
                      << "  --algo1 NAME     First algorithm for detailed comparison\n"
                      << "  --algo2 NAME     Second algorithm for detailed comparison\n"
                      << "  --help           Show this help message\n";
            return 0;
        }
    }
    
    // 알고리즘 정의
    std::vector<AlgorithmInfo> all_algorithms = {
        {"Random", randomAction},
        {"Minimax", [simulation_count](const TwoMazeState& state) { 
            return miniMaxSearchAction(state, 4); // 깊이 4로 고정
        }},
        {"AlphaBeta", [simulation_count](const TwoMazeState& state) { 
            return alphaBetaSearchAction(state, 4); // 깊이 4로 고정
        }},
        {"Deepening", [time_threshold](const TwoMazeState& state) { 
            return iterativeDeepeningSearchAction(state, time_threshold);
        }},
        {"MonteCarlo", [simulation_count](const TwoMazeState& state) { 
            return monteCarloSearchAction(state, simulation_count);
        }},
        {"MCTS", [simulation_count](const TwoMazeState& state) { 
            return mctsSearchAction(state, simulation_count);
        }},
        {"Thunder", [simulation_count](const TwoMazeState& state) { 
            return thunderSearchAction(state, simulation_count);
        }}
    };
    
    // 시간 제한 기반 알고리즘 & 시간은 나중에 조정됨
    std::vector<AlgorithmInfo> time_algorithms = {
        {"MCTS_Time", [](const TwoMazeState& state) { 
            return mctsSearchAction(state, 100);
        }},
        {"Thunder_Time", [](const TwoMazeState& state) { 
            return thunderSearchActionWithTime(state, 100);
        }},
        {"Deepening_Time", [](const TwoMazeState& state) { 
            return iterativeDeepeningSearchAction(state, 100);
        }}
    };
    
    std::vector<int64_t> time_limits = {1, 10, 50, 100, 250, 500, 1000};
    
    // 벤치마크 모드에 따라 실행
    if (benchmark_mode == "all" || benchmark_mode == "full") {
        runFullBenchmark(all_algorithms, game_count);
    } else if (benchmark_mode == "time") {
        // 시간 제한 기반 분석 - 기준 알고리즘은 랜덤으로 설정
        analyzeTimeConstraints(time_algorithms, all_algorithms[0], time_limits, game_count);
    } else {
        std::cout << "알 수 없는 모드: " << benchmark_mode << "\n";
        return 1;
    }
    
    return 0;
}