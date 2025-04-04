#include "../algorithms/algorithm_interface.h"
#include "../games/twomaze/twomaze_state.h"
#include "../common/coord.h"
#include "../common/game_util.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <chrono>
#include <memory>
#include <functional>

// 알고리즘별 적절한 파라미터 설정 유틸리티 함수 
void configureAlgorithmParams(const std::string& algo_name, AlgorithmParams& params, int simulation_count, int search_depth, int64_t time_ms) {
    if (algo_name == "Minimax" || algo_name == "AlphaBeta") {
        params.searchDepth = search_depth;
    } else if (algo_name == "IterativeDeepening") {
        params.timeThreshold = time_ms; // 100ms
    } else if (algo_name == "MonteCarlo" || algo_name == "MCTS" || algo_name == "Thunder") {
        params.playoutNumber = simulation_count;
    } else if (algo_name == "ThunderTime") {
        params.timeThreshold = time_ms; // 100ms
    }
}

// 두 알고리즘의 승률 비교
void compareAlgorithms(const std::string& ai1_name, const std::string& ai2_name, int game_count, int simulation_count, int search_depth, int64_t time_ms) {
    // 알고리즘 1의 파라미터 설정
    AlgorithmParams params1;
    configureAlgorithmParams(ai1_name, params1, simulation_count, search_depth, time_ms);
    auto ai1 = AlgorithmFactory::createAlgorithm(ai1_name, params1);
    
    // 알고리즘 2의 파라미터 설정
    AlgorithmParams params2;
    configureAlgorithmParams(ai2_name, params2, simulation_count, search_depth, time_ms);
    auto ai2 = AlgorithmFactory::createAlgorithm(ai2_name, params2);
    
    double ai1_win_rate = 0.0;
    int total_games = 0;
    
    std::cout << "비교 중: " << ai1->getName() << " vs " << ai2->getName() << " (" << game_count << " 게임)" << std::endl;
    std::cout << "진행 중: ";
    std::cout.flush();
    
    for (int i = 0; i < game_count; i++) {
        auto base_state = std::make_unique<TwoMazeState>(i);
        
        // 선공/후공을 교대로 진행
        for (int j = 0; j < 2; j++) {
            auto state = std::make_unique<TwoMazeState>(*base_state);
            
            auto& first_player = (j == 0) ? ai1 : ai2;
            auto& second_player = (j == 0) ? ai2 : ai1;
            
            while (!state->isDone()) {
                state->progress(first_player->selectAction(*state));
                if (state->isDone()) break;
                
                state->progress(second_player->selectAction(*state));
                if (state->isDone()) break;
            }
            
            WinningStatus status = state->getWinningStatus();
            bool is_first_player_win = (status == WinningStatus::WIN);
            bool is_draw = (status == WinningStatus::DRAW);
            
            double win_point = 0.0;
            if (is_draw) {
                win_point = 0.5;
            } else if ((j == 0 && is_first_player_win) || (j == 1 && !is_first_player_win)) {
                win_point = 1.0;
            }
            
            ai1_win_rate += win_point;
            total_games++;
            
            if (total_games % 10 == 0) {
                std::cout << ".";
                std::cout.flush();
            }
        }
    }
    
    ai1_win_rate /= total_games;
    std::cout << std::endl;
    std::cout << ai1->getName() << " 승률: " << std::fixed << std::setprecision(2) 
              << (ai1_win_rate * 100) << "%" << std::endl;
    std::cout << ai2->getName() << " 승률: " << std::fixed << std::setprecision(2) 
              << ((1.0 - ai1_win_rate) * 100) << "%" << std::endl;
}

int main(int argc, char* argv[]) {
    // (설정)
    std::string algo1_name = "Thunder";
    std::string algo2_name = "AlphaBeta";
    int game_count = 50;
    int simulation_count = 1000;
    int search_depth = 4;
    int64_t time_ms = 100;
    
    // 알고리즘 매핑 (커맨드라인 -> 내부 이름)
    std::map<std::string, std::string> algorithm_map = {
        {"random", "TwoMazeRandom"},
        {"minimax", "Minimax"},
        {"alphabeta", "AlphaBeta"},
        {"deepening", "IterativeDeepening"},
        {"mc", "MonteCarlo"},
        {"mcts", "MCTS"},
        {"thunder", "Thunder"},
        {"thunder_time", "ThunderTime"}
    };
    
    // 명령줄 인자 처리
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--algo1" && i + 1 < argc) {
            algo1_name = algorithm_map[argv[++i]];
        } else if (arg == "--algo2" && i + 1 < argc) {
            algo2_name = algorithm_map[argv[++i]];
        } else if (arg == "--games" && i + 1 < argc) {
            game_count = std::stoi(argv[++i]);
        } else if (arg == "--sims" && i + 1 < argc) {
            simulation_count = std::stoi(argv[++i]);
        } else if (arg == "--time" && i + 1 < argc) {
            time_ms = std::stol(argv[++i]);
        } else if (arg == "--help") {
            std::cout << "사용법: twomaze_battle [options]\n"
                      << "옵션:\n"
                      << "  --algo1 NAME     First algorithm name\n"
                      << "  --algo2 NAME     Second algorithm name\n"
                      << "  --games N        Number of games to play; default: 50\n"
                      << "  --sims N         Number of simulations for MC-based algorithms; default: 1000\n"
                      << "  --time N         Time threshold in milliseconds; default: 100\n"
                      << "  --help           Show this help message\n"
                      << "\n사용 가능한 알고리즘: Random, Minimax, AlphaBeta, Iterative, MonteCarlo, MCTS, Thunder, Thunder_Time\n";
            return 0;
        }
    }
    
    try {
        AlgorithmParams testParams;
        auto test1 = AlgorithmFactory::createAlgorithm(algo1_name, testParams);
        auto test2 = AlgorithmFactory::createAlgorithm(algo2_name, testParams);
        
        compareAlgorithms(algo1_name, algo2_name, game_count, simulation_count, search_depth, time_ms);
    } catch (const std::invalid_argument& e) {
        std::cout << "오류: " << e.what() << std::endl;
        std::cout << "사용 가능한 알고리즘: ";
        for (const auto& pair : algorithm_map) {
            std::cout << pair.first << " ";
        }
        std::cout << std::endl;
        return 1;
    }
    
    return 0;
}