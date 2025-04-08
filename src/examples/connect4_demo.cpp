#include "../algorithms/algorithm_interface.h"
#include "../games/connect_four/connect_four_state.h"
#include "../games/connect_four/connect_four_bitboard.h"
#include "../common/game_util.h"
#include <iostream>
#include <string>
#include <map>
#include <array>
#include <chrono>
#include <functional>
#include <unordered_map>

void playGameWithAlgorithms(const std::string& algo1_name, const std::string& algo2_name, int seed = 0) {
    
    // 알고리즘 파라미터 설정 (선택)
    AlgorithmParams params1, params2;
    params1.timeThreshold = 100; // 100ms
    params2.timeThreshold = 100; // 100ms
    
    auto algo1 = AlgorithmFactory::createAlgorithm(algo1_name, params1);
    auto algo2 = AlgorithmFactory::createAlgorithm(algo2_name, params2);
    
    std::cout << "Game Start: " << algo1->getName() << " vs " << algo2->getName() << std::endl;
    
    auto state = std::make_unique<ConnectFourState>(seed);
    
    std::cout << state->toString() << std::endl;
    
    while (!state->isDone()) {
        int action;
        std::string current_player;
        
        // 현재 플레이어 결정 (첫 번째 플레이어는 x, 두 번째 플레이어는 o)
        if (state->isFirst()) {
            action = algo1->selectAction(*state);
            current_player = "Player 1 (x)";
        } else {
            action = algo2->selectAction(*state);
            current_player = "Player 2 (o)";
        }
        
        std::cout << current_player << " 행동: " << action << std::endl;
        
        state->progress(action);
        std::cout << state->toString() << std::endl;
    }
    
    std::cout << "Game End!" << std::endl;
    
    double result = state->getFirstPlayerScoreForWinRate();
    if (result == 1.0) {
        std::cout << "Player 1 (x) Wins!" << std::endl;
    } else if (result == 0.0) {
        std::cout << "Player 2 (o) Wins!" << std::endl;
    } else {
        std::cout << "Draw!" << std::endl;
    }
}

// 알고리즘 성능 테스트
void benchmarkAlgorithms(const std::string& algo1_name, const std::string& algo2_name, int game_count = 10) {
    AlgorithmParams params1, params2;
    params1.timeThreshold = 100; // 100ms
    params2.timeThreshold = 100; // 100ms
    
    auto algo1 = AlgorithmFactory::createAlgorithm(algo1_name, params1);
    auto algo2 = AlgorithmFactory::createAlgorithm(algo2_name, params2);
    
    std::cout << "Benchmark Start: " << algo1->getName() << " vs " << algo2->getName()
              << " (" << game_count << " games)" << std::endl;
    
    int algo1_wins = 0;
    int algo2_wins = 0;
    int draws = 0;
    
    for (int i = 0; i < game_count; i++) {
        std::cout << "Game " << (i + 1) << "/" << game_count << " in progress..." << std::endl;
        
        auto state = std::make_unique<ConnectFourState>(i); // 각 게임마다 다른 시드 사용
        
        while (!state->isDone()) {
            int action;
            
            if (state->isFirst()) {
                action = algo1->selectAction(*state);
            } else {
                action = algo2->selectAction(*state);
            }
            
            state->progress(action);
        }
        
        double result = state->getFirstPlayerScoreForWinRate();
        if (result == 1.0) {
            algo1_wins++;
            std::cout << "Player 1 Wins" << std::endl;
        } else if (result == 0.0) {
            algo2_wins++;
            std::cout << "Player 2 Wins" << std::endl;
        } else {
            draws++;
            std::cout << "Draw" << std::endl;
        }
    }
    
    std::cout << "\n===== Benchmark Result =====" << std::endl;
    std::cout << algo1->getName() << " Wins: " << algo1_wins 
              << " (" << (algo1_wins * 100.0 / game_count) << "%)" << std::endl;
    std::cout << algo2->getName() << " Wins: " << algo2_wins 
              << " (" << (algo2_wins * 100.0 / game_count) << "%)" << std::endl;
    std::cout << "Draw: " << draws 
              << " (" << (draws * 100.0 / game_count) << "%)" << std::endl;
}

void compareBitboardPerformance(int simulation_count = 1000) {
    std::cout << "Bitboard Optimization Performance Comparison (" << simulation_count << " simulations)" << std::endl;
    
    AlgorithmParams params;
    params.timeThreshold = 1000; // 1초
    
    auto mcts_algo = AlgorithmFactory::createAlgorithm("ConnectFourMCTS", params);
    auto bit_mcts_algo = AlgorithmFactory::createAlgorithm("ConnectFourBitMCTS", params);
    
    auto state = std::make_unique<ConnectFourState>(0);
    
    // 일반 MCTS 실행 시간 측정
    auto start_time = std::chrono::high_resolution_clock::now();
    int action1 = mcts_algo->selectAction(*state);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    
    // 비트보드 MCTS 실행 시간 측정
    start_time = std::chrono::high_resolution_clock::now();
    int action2 = bit_mcts_algo->selectAction(*state);
    end_time = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    
    std::cout << "\n===== Performance Comparison Result =====" << std::endl;
    std::cout << "Normal MCTS: " << duration1 << "ms" << std::endl;
    std::cout << "Bitboard MCTS: " << duration2 << "ms" << std::endl;
    std::cout << "Speed Improvement: " << ((duration1 - duration2) * 100.0 / duration1) << "%" << std::endl;
    
    std::cout << "\nSelected Action Comparison:" << std::endl;
    std::cout << "Normal MCTS: " << action1 << std::endl;
    std::cout << "Bitboard MCTS: " << action2 << std::endl;
}

int getHumanMove(const ConnectFourState& state) {
    int column;
    
    while (true) {
        std::cout << "Enter the column number (1-7): ";
        std::cin >> column;
        
        // 1-7 입력을 0-6 인덱스로 변환
        column--;
        
        // 입력 유효성 검사
        if (column >= 0 && column < GameConstants::ConnectFour::W) {
            auto legal_actions = state.legalActions();
            if (std::find(legal_actions.begin(), legal_actions.end(), column) != legal_actions.end()) {
                return column;
            }
        }
        
        std::cout << "Invalid input. Please try again." << std::endl;
    }
}

// 사람 vs AI 게임 실행 함수
void playHumanVsAI(const std::string& ai_algorithm, int difficulty = 0) {
    int seed = GameUtil::mt_for_action();
    AlgorithmParams params;

    switch (difficulty) {
        case 0: // 쉬움
            params.timeThreshold = 100;
            params.playoutNumber = 500;
            break;
        case 1: // 보통
            params.timeThreshold = 500;
            params.playoutNumber = 1000;
            break;
        case 2: // 어려움
            params.timeThreshold = 1000;
            params.playoutNumber = 2000;
            break;
        default:
            break;
    }
    
    auto ai = AlgorithmFactory::createAlgorithm(ai_algorithm, params);
    
    std::cout << "=== Human vs " << ai->getName() << " ===" << std::endl;
    
    auto state = std::make_unique<ConnectFourState>(seed);
    
    std::cout << state->toString() << std::endl;
    
    while (!state->isDone()) {
        int action;
        
        if (state->isFirst()) {
            // 사람 차례 (X)
            action = getHumanMove(*state);
            std::cout << "Your move: " << (action + 1) << std::endl;
        } else {
            // AI 차례 (O)
            std::cout << "AI is thinking..." << std::endl;
            
            auto start = std::chrono::high_resolution_clock::now();
            action = ai->selectAction(*state);
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << "AI's move: " << (action + 1) << " (calculation time: " << duration << "ms)" << std::endl;
        }
        
        state->progress(action);
        std::cout << state->toString() << std::endl;
    }
    
    double result = state->getFirstPlayerScoreForWinRate();
    if (result == 1.0) {
        std::cout << "Congratulations! You win! 🎉" << std::endl;
    } else if (result == 0.0) {
        std::cout << "AI wins! 😢" << std::endl;
    } else {
        std::cout << "Draw! 🤝" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    GameUtil::mt_for_action.seed(0);
    
    std::string mode = "play";
    std::string algo1 = "ConnectFourRandom";
    std::string algo2 = "ConnectFourMCTS";
    int games = 10;
    int difficulty = 0;
    
    std::unordered_map<std::string, std::string> algo_map = {
        {"random", "ConnectFourRandom"},
        {"mcts", "ConnectFourMCTS"},
        {"bitmcts", "ConnectFourBitMCTS"},
        {"id", "ConnectFourID"},
        {"bitid", "ConnectFourBitID"}
    };

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--mode" && i + 1 < argc) {
            mode = argv[++i];
        } else if (arg == "--algo1" && i + 1 < argc) {
            std::string algo_name = argv[++i];
            if (algo_map.find(algo_name) != algo_map.end()) {
                algo1 = algo_map[algo_name];
            } else {
                std::cout << "Unknown algorithm: " << algo_name << std::endl;
                std::cout << "Available algorithms: ";
                for (const auto& pair : algo_map) {
                    std::cout << pair.first << " ";
                }
                std::cout << std::endl;
                return 1;
            }
        } else if (arg == "--algo2" && i + 1 < argc) {
            std::string algo_name = argv[++i];
            if (algo_map.find(algo_name) != algo_map.end()) {
                algo2 = algo_map[algo_name];
            } else {
                std::cout << "Unknown algorithm: " << algo_name << std::endl;
                std::cout << "Available algorithms: ";
                for (const auto& pair : algo_map) {
                    std::cout << pair.first << " ";
                }
                std::cout << std::endl;
                return 1;
            }
        } else if (arg == "--games" && i + 1 < argc) {
            games = std::stoi(argv[++i]);
        } else if (arg == "--difficulty" && i + 1 < argc) {
            difficulty = std::stoi(argv[++i]);
        } else if (arg == "--help") {
            std::cout << "Usage: connect_four_demo [options]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  --mode MODE     Execution mode (play, benchmark, compare, human)" << std::endl;
            std::cout << "  --algo1 ALGO    First algorithm (random, mcts, bitmcts, id, bitid)" << std::endl;
            std::cout << "  --algo2 ALGO    Second algorithm (random, mcts, bitmcts, id, bitid)" << std::endl;
            std::cout << "  --games N       Number of games to run in benchmark mode" << std::endl;
            std::cout << "  --difficulty D  Human vs AI game difficulty (0: easy, 1: normal, 2: hard)" << std::endl;
            std::cout << "  --help          Display this help message" << std::endl;
            return 0;
        }
    }
    
    if (mode == "play") {
        playGameWithAlgorithms(algo1, algo2);
    } else if (mode == "benchmark") {
        benchmarkAlgorithms(algo1, algo2, games);
    } else if (mode == "compare") {
        compareBitboardPerformance();
    } else if (mode == "human") {
        playHumanVsAI(algo1, difficulty);
    } else {
        std::cout << "Unknown mode: " << mode << std::endl;
        std::cout << "Available modes: play, benchmark, compare" << std::endl;
        return 1;
    }
    
    return 0;
}