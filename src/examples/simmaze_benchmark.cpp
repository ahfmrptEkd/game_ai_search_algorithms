#include "../games/simmaze/simmaze_state.h"
#include "../common/coord.h"
#include "../common/game_util.h"
#include "../algorithms/two_player/simultaneous/random.h"
#include "../algorithms/two_player/simultaneous/duct.h"
#include "../algorithms/two_player/simultaneous/pmc.h"
#include "../algorithms/two_player/simultaneous/mcts_sim.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <chrono>
#include <functional>
#include <algorithm>

// 알고리즘 성능 결과 구조체
struct AlgorithmPerformance {
    double win_rate = 0.0;         // 승률
    double avg_score = 0.0;        // 평균 점수
    double avg_time_ms = 0.0;      // 평균 실행 시간(ms)
    int total_games = 0;           // 총 게임 수
};

// 알고리즘 선택 함수 타입 정의
using AlgorithmFunction = std::function<int(const SimMazeState&, const int)>;

// 알고리즘 선택 함수 생성
AlgorithmFunction createAlgorithm(const std::string& name, int simulation_number) {
    if (name == "random") {
        return simMazeRandomAction;
    } else if (name == "duct") {
        return [simulation_number](const SimMazeState& state, const int player_id) {
            return ductSearchAction(state, player_id, simulation_number);
        };
    } else if (name == "pmc") {
        return [simulation_number](const SimMazeState& state, const int player_id) {
            return pmcSearchAction(state, player_id, simulation_number);
        };
    } else if (name == "mcts") {
        return [simulation_number](const SimMazeState& state, const int player_id) {
            return mctsSimSearchAction(state, player_id, simulation_number);
        };
    } else {
        std::cerr << "Unknown algorithm: " << name << ". Using Random instead." << std::endl;
        return simMazeRandomAction;
    }
}

// 단일 대결 벤치마크 (자세한 정보 출력)
AlgorithmPerformance benchmarkDuel(
    const std::string& algo1_name, 
    const std::string& algo2_name,
    int games,
    int simulation_number,
    bool verbose = true
) {
    auto algo1 = createAlgorithm(algo1_name, simulation_number);
    auto algo2 = createAlgorithm(algo2_name, simulation_number);
    
    AlgorithmPerformance result;
    result.total_games = games;
    
    double total_score = 0.0;
    double total_time_ms = 0.0;
    double wins = 0.0;
    
    if (verbose) {
        std::cout << algo1_name << " vs " << algo2_name << " (" << games << " games, " 
                  << simulation_number << " simulations)" << std::endl;
    }
    
    for (int game = 0; game < games; game++) {
        SimMazeState state(game); // 시드로 게임 번호 사용
        
        auto start_time = std::chrono::high_resolution_clock::now();
        
        while (!state.isDone()) {
            // 플레이어 1 (algo1) 행동
            auto p1_start = std::chrono::high_resolution_clock::now();
            int action1 = algo1(state, 0);
            auto p1_end = std::chrono::high_resolution_clock::now();
            
            // 플레이어 2 (algo2) 행동
            int action2 = algo2(state, 1);
            
            // 시간 측정 (플레이어 1의 시간만 측정)
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(p1_end - p1_start).count();
            total_time_ms += duration;
            
            // 게임 진행
            state.advance(action1, action2);
        }
        
        // 결과 평가
        WinningStatus status = state.getWinningStatus();
        double game_result = 0.0;
        
        if (status == WinningStatus::WIN) {
            game_result = 1.0;  // 승리
        } else if (status == WinningStatus::DRAW) {
            game_result = 0.5;  // 무승부
        }
        
        wins += game_result;
        total_score += state.getPlayerScore(0);  // 플레이어 1 점수
        
        if (verbose && (game + 1) % 10 == 0) {
            std::cout << "  Progress: " << game + 1 << "/" << games 
                      << " games, Win rate: " << std::fixed << std::setprecision(2) 
                      << (wins * 100.0 / (game + 1)) << "%" << std::endl;
        }
    }
    
    // 결과 계산
    result.win_rate = wins / games;
    result.avg_score = total_score / games;
    result.avg_time_ms = total_time_ms / games;
    
    if (verbose) {
        std::cout << "  Final win rate: " << std::fixed << std::setprecision(2) 
                  << (result.win_rate * 100.0) << "%" << std::endl;
        std::cout << "  Average score: " << std::fixed << std::setprecision(2) 
                  << result.avg_score << std::endl;
        std::cout << "  Average time: " << std::fixed << std::setprecision(2) 
                  << result.avg_time_ms << " ms" << std::endl;
        std::cout << std::endl;
    }
    
    return result;
}

// 모든 알고리즘 조합 벤치마크
void benchmarkAllCombinations(
    const std::vector<std::string>& algorithms, 
    int games_per_match, 
    int simulation_number
) {
    std::cout << "\n===== 알고리즘 종합 벤치마크 =====" << std::endl;
    std::cout << "각 대결 " << games_per_match << "회, 시뮬레이션 " << simulation_number << "회" << std::endl << std::endl;
    
    // 결과 행렬 초기화
    std::vector<std::vector<double>> win_rates(algorithms.size(), 
                                               std::vector<double>(algorithms.size(), 0.0));
    
    // 테이블 헤더 출력
    std::cout << std::left << std::setw(10) << "Algorithm";
    for (const auto& algo : algorithms) {
        std::cout << std::setw(10) << algo;
    }
    std::cout << std::setw(10) << "평균 승률" << std::endl;
    
    // 구분선
    std::cout << std::string(10 + algorithms.size() * 10 + 10, '-') << std::endl;
    
    // 모든 알고리즘 조합에 대해 벤치마크 실행
    for (size_t i = 0; i < algorithms.size(); i++) {
        std::cout << std::left << std::setw(10) << algorithms[i];
        
        double total_win_rate = 0.0;
        int opponent_count = 0;
        
        for (size_t j = 0; j < algorithms.size(); j++) {
            if (i == j) {
                // 같은 알고리즘끼리는 테스트하지 않음
                std::cout << std::setw(10) << "-";
                continue;
            }
            
            // 벤치마크 실행 (상세 출력 없이)
            AlgorithmPerformance result = benchmarkDuel(
                algorithms[i], algorithms[j], games_per_match, simulation_number, false
            );
            
            win_rates[i][j] = result.win_rate;
            
            // 승률 출력
            std::cout << std::right << std::setw(8) << std::fixed << std::setprecision(1) 
                      << (result.win_rate * 100.0) << "% ";
            
            total_win_rate += result.win_rate;
            opponent_count++;
        }
        
        // 평균 승률 계산
        double avg_win_rate = opponent_count > 0 ? total_win_rate / opponent_count : 0.0;
        std::cout << std::right << std::setw(9) << std::fixed << std::setprecision(1) 
                  << (avg_win_rate * 100.0) << "%" << std::endl;
    }
    
    std::cout << std::endl;
}

// 시간 제한 기반 알고리즘 성능 분석
void benchmarkTimeConstraints(
    const std::vector<std::string>& time_based_algorithms,
    const std::string& baseline_algorithm,
    const std::vector<int>& time_limits,
    int games_per_time,
    int simulation_number
) {
    std::cout << "\n===== 시간 제한 기반 성능 분석 =====" << std::endl;
    std::cout << "기준 알고리즘: " << baseline_algorithm << std::endl;
    std::cout << "각 시간 제한당 " << games_per_time << "회 게임" << std::endl << std::endl;
    
    // 테이블 헤더 출력
    std::cout << std::left << std::setw(10) << "Algorithm";
    for (int time_ms : time_limits) {
        std::cout << std::setw(10) << (std::to_string(time_ms) + "ms");
    }
    std::cout << std::endl;
    
    // 구분선
    std::cout << std::string(10 + time_limits.size() * 10, '-') << std::endl;
    
    // 각 알고리즘별 시간 제한 테스트
    for (const auto& algo : time_based_algorithms) {
        std::cout << std::left << std::setw(10) << algo;
        
        for (int time_ms : time_limits) {
            // 시간 제한에 맞춰 시뮬레이션 횟수 조정
            int adjusted_sims = simulation_number * time_ms / 100; // 기준 100ms
            
            // 벤치마크 실행 (상세 출력 없이)
            AlgorithmPerformance result = benchmarkDuel(
                algo, baseline_algorithm, games_per_time, adjusted_sims, false
            );
            
            // 승률 출력
            std::cout << std::right << std::setw(8) << std::fixed << std::setprecision(1) 
                      << (result.win_rate * 100.0) << "% ";
        }
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
}

// 도움말 출력
void printUsage() {
    std::cout << "사용법: simmaze_benchmark [명령] [옵션]" << std::endl;
    std::cout << "명령:" << std::endl;
    std::cout << "  all                      - 모든 알고리즘 조합 벤치마크" << std::endl;
    std::cout << "  duel <algo1> <algo2>     - 두 알고리즘 대결 벤치마크" << std::endl;
    std::cout << "  time                     - 시간 제한 기반 성능 분석" << std::endl;
    std::cout << "  help                     - 도움말 출력" << std::endl;
    std::cout << std::endl;
    std::cout << "옵션:" << std::endl;
    std::cout << "  --games N                - 각 대결당 게임 수 (기본값: 100)" << std::endl;
    std::cout << "  --sims N                 - 시뮬레이션 횟수 (기본값: 1000)" << std::endl;
    std::cout << std::endl;
    std::cout << "사용 가능한 알고리즘: random, pmc, mcts, duct" << std::endl;
}

// 메인 함수
int main(int argc, char* argv[]) {
    GameUtil::mt_for_action.seed(0);
    
    // 기본 설정값
    std::string command = "all";
    std::string algo1 = "random";
    std::string algo2 = "duct";
    int games = 100;
    int simulation_number = 1000;
    
    // 사용 가능한 알고리즘 목록
    std::vector<std::string> algorithms = {"random", "pmc", "mcts", "duct"};
    
    // 시간 제한 테스트용 알고리즘 (계산 집약적인 알고리즘만)
    std::vector<std::string> time_algorithms = {"pmc", "mcts", "duct"};
    
    // 시간 제한 목록 (밀리초)
    std::vector<int> time_limits = {10, 50, 100, 250, 500, 1000};
    
    // 명령행 인자 처리
    if (argc > 1) {
        command = argv[1];
        
        if (command == "help") {
            printUsage();
            return 0;
        }
    }
    
    // 추가 옵션 처리
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--games" && i + 1 < argc) {
            games = std::stoi(argv[++i]);
        } else if (arg == "--sims" && i + 1 < argc) {
            simulation_number = std::stoi(argv[++i]);
        } else if (command == "duel" && i <= 3) {
            // duel 명령의 경우 알고리즘 이름 처리
            if (i == 2) algo1 = arg;
            else if (i == 3) algo2 = arg;
        }
    }
    
    // 명령 실행
    if (command == "all") {
        benchmarkAllCombinations(algorithms, games, simulation_number);
    } else if (command == "duel") {
        std::cout << "===== 알고리즘 대결 벤치마크 =====" << std::endl;
        benchmarkDuel(algo1, algo2, games, simulation_number);
    } else if (command == "time") {
        benchmarkTimeConstraints(time_algorithms, "random", time_limits, games / 3, simulation_number);
    } else {
        std::cout << "알 수 없는 명령: " << command << std::endl;
        printUsage();
        return 1;
    }
    
    return 0;
}