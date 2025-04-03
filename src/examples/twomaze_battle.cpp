// src/examples/twomaze_battle.cpp
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
#include <chrono>
#include <functional>

using AIFunction = std::function<int(const TwoMazeState&)>;
using StringAIPair = std::pair<std::string, AIFunction>;

// 두 알고리즘의 승률 비교
void compareAlgorithms(const StringAIPair& ai1, const StringAIPair& ai2, int game_count) {
    double ai1_win_rate = 0.0;
    int total_games = 0;
    
    std::cout << "비교 중: " << ai1.first << " vs " << ai2.first << " (" << game_count << " 게임)" << std::endl;
    std::cout << "진행 중: ";
    std::cout.flush();
    
    for (int i = 0; i < game_count; i++) {
        auto base_state = TwoMazeState(i);
        
        // 선공/후공을 교대로 진행
        for (int j = 0; j < 2; j++) {
            auto state = base_state;
            auto& first_player = (j == 0) ? ai1 : ai2;
            auto& second_player = (j == 0) ? ai2 : ai1;
            
            while (!state.isDone()) {
                state.progress(first_player.second(state));
                if (state.isDone()) break;
                
                state.progress(second_player.second(state));
                if (state.isDone()) break;
            }
            
            WinningStatus status = state.getWinningStatus();
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
    std::cout << ai1.first << " 승률: " << std::fixed << std::setprecision(2) 
              << (ai1_win_rate * 100) << "%" << std::endl;
    std::cout << ai2.first << " 승률: " << std::fixed << std::setprecision(2) 
              << ((1.0 - ai1_win_rate) * 100) << "%" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string algo1_name = "Thunder";
    std::string algo2_name = "AlphaBeta";
    int game_count = 50;
    int simulation_count = 1000;
    int64_t time_ms = 100;
    
    // 명령줄 인자 처리
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--algo1" && i + 1 < argc) {
            algo1_name = argv[++i];
        } else if (arg == "--algo2" && i + 1 < argc) {
            algo2_name = argv[++i];
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
                      << "  --games N        Number of games to play\n"
                      << "  --sims N         Number of simulations for MC-based algorithms\n"
                      << "  --time N         Time threshold in milliseconds\n"
                      << "  --help           Show this help message\n"
                      << "\n사용 가능한 알고리즘: Random, Minimax, AlphaBeta, Iterative, MonteCarlo, MCTS, Thunder, Thunder_Time\n";
            return 0;
        }
    }
    
    std::map<std::string, AIFunction> algorithms = {
        {"Random", randomAction},
        {"Minimax", [](const TwoMazeState& state) { 
            return miniMaxSearchAction(state, 4); 
        }},
        {"AlphaBeta", [](const TwoMazeState& state) { 
            return alphaBetaSearchAction(state, 4); 
        }},
        {"Iterative", [time_ms](const TwoMazeState& state) { 
            return iterativeDeepeningSearchAction(state, time_ms); 
        }},
        {"MonteCarlo", [simulation_count](const TwoMazeState& state) { 
            return monteCarloSearchAction(state, simulation_count); 
        }},
        {"MCTS", [simulation_count](const TwoMazeState& state) { 
            return mctsSearchAction(state, simulation_count); 
        }},
        {"Thunder", [simulation_count](const TwoMazeState& state) { 
            return thunderSearchAction(state, simulation_count); 
        }},
        {"Thunder_Time", [time_ms](const TwoMazeState& state) { 
            return thunderSearchActionWithTime(state, time_ms); 
        }}
    };
    
    if (algorithms.find(algo1_name) == algorithms.end() || 
        algorithms.find(algo2_name) == algorithms.end()) {
        std::cout << "오류: 알고리즘을 찾을 수 없습니다. 사용 가능한 알고리즘:\n";
        for (const auto& pair : algorithms) {
            std::cout << "  " << pair.first << "\n";
        }
        return 1;
    }
    
    StringAIPair ai1(algo1_name, algorithms[algo1_name]);
    StringAIPair ai2(algo2_name, algorithms[algo2_name]);
    
    compareAlgorithms(ai1, ai2, game_count);
    
    return 0;
}