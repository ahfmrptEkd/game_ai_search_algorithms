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

void playGameWithAlgorithms(const std::string& algo1_name, const std::string& algo2_name, int seed = 0) {
    // 알고리즘 파라미터 설정
    AlgorithmParams params1, params2;
    params1.timeThreshold = 100; // 100ms
    params2.timeThreshold = 100; // 100ms
    
    auto algo1 = AlgorithmFactory::createAlgorithm(algo1_name, params1);
    auto algo2 = AlgorithmFactory::createAlgorithm(algo2_name, params2);
    
    std::cout << "게임 시작: " << algo1->getName() << " vs " << algo2->getName() << std::endl;
    
    // 게임 상태 초기화
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
    
    std::cout << "게임 종료!" << std::endl;
    
    double result = state->getFirstPlayerScoreForWinRate();
    if (result == 1.0) {
        std::cout << "Player 1 (x) 승리!" << std::endl;
    } else if (result == 0.0) {
        std::cout << "Player 2 (o) 승리!" << std::endl;
    } else {
        std::cout << "무승부!" << std::endl;
    }
}

// 알고리즘 성능 테스트
void benchmarkAlgorithms(const std::string& algo1_name, const std::string& algo2_name, int game_count = 10) {
    AlgorithmParams params1, params2;
    params1.timeThreshold = 100; // 100ms
    params2.timeThreshold = 100; // 100ms
    
    auto algo1 = AlgorithmFactory::createAlgorithm(algo1_name, params1);
    auto algo2 = AlgorithmFactory::createAlgorithm(algo2_name, params2);
    
    std::cout << "벤치마크 시작: " << algo1->getName() << " vs " << algo2->getName()
              << " (" << game_count << " 게임)" << std::endl;
    
    int algo1_wins = 0;
    int algo2_wins = 0;
    int draws = 0;
    
    for (int i = 0; i < game_count; i++) {
        std::cout << "게임 " << (i + 1) << "/" << game_count << " 진행 중..." << std::endl;
        
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
            std::cout << "Player 1 승리" << std::endl;
        } else if (result == 0.0) {
            algo2_wins++;
            std::cout << "Player 2 승리" << std::endl;
        } else {
            draws++;
            std::cout << "무승부" << std::endl;
        }
    }
    
    // 결과 요약
    std::cout << "\n===== 벤치마크 결과 =====" << std::endl;
    std::cout << algo1->getName() << " 승리: " << algo1_wins 
              << " (" << (algo1_wins * 100.0 / game_count) << "%)" << std::endl;
    std::cout << algo2->getName() << " 승리: " << algo2_wins 
              << " (" << (algo2_wins * 100.0 / game_count) << "%)" << std::endl;
    std::cout << "무승부: " << draws 
              << " (" << (draws * 100.0 / game_count) << "%)" << std::endl;
}

void compareBitboardPerformance(int simulation_count = 1000) {
    std::cout << "비트보드 최적화 성능 비교 (" << simulation_count << " 시뮬레이션)" << std::endl;
    
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
    
    std::cout << "\n===== 성능 비교 결과 =====" << std::endl;
    std::cout << "일반 MCTS: " << duration1 << "ms" << std::endl;
    std::cout << "비트보드 MCTS: " << duration2 << "ms" << std::endl;
    std::cout << "속도 향상: " << ((duration1 - duration2) * 100.0 / duration1) << "%" << std::endl;
    
    // 선택한 행동 확인
    std::cout << "\n선택한 행동 비교:" << std::endl;
    std::cout << "일반 MCTS: " << action1 << std::endl;
    std::cout << "비트보드 MCTS: " << action2 << std::endl;
}

int getHumanMove(const ConnectFourState& state) {
    int column;
    
    while (true) {
        std::cout << "열 번호를 입력하세요 (1-7): ";
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
        
        std::cout << "유효하지 않은 수입니다. 다시 시도하세요." << std::endl;
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
    
    std::cout << "=== 사람 vs " << ai->getName() << " ===" << std::endl;
    
    // 게임 상태 초기화
    auto state = std::make_unique<ConnectFourState>(seed);
    
    std::cout << state->toString() << std::endl;
    
    while (!state->isDone()) {
        int action;
        
        if (state->isFirst()) {
            // 사람 차례 (X)
            action = getHumanMove(*state);
            std::cout << "당신의 수: " << (action + 1) << std::endl;
        } else {
            // AI 차례 (O)
            std::cout << "AI가 생각하는 중..." << std::endl;
            
            auto start = std::chrono::high_resolution_clock::now();
            action = ai->selectAction(*state);
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << "AI의 수: " << (action + 1) << " (계산 시간: " << duration << "ms)" << std::endl;
        }
        
        state->progress(action);
        std::cout << state->toString() << std::endl;
    }
    
    // 게임 결과 출력
    double result = state->getFirstPlayerScoreForWinRate();
    if (result == 1.0) {
        std::cout << "축하합니다! 당신이 이겼습니다! 🎉" << std::endl;
    } else if (result == 0.0) {
        std::cout << "AI가 이겼습니다! 😢" << std::endl;
    } else {
        std::cout << "무승부입니다! 🤝" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    GameUtil::mt_for_action.seed(0);
    
    // 커맨드 라인 인자 처리
    std::string mode = "play";
    std::string algo1 = "ConnectFourRandom";
    std::string algo2 = "ConnectFourMCTS";
    int games = 10;
    int difficulty = 0;
    
    std::map<std::string, std::string> algorithms = {
        {"random", "ConnectFourRandom"},
        {"mcts", "ConnectFourMCTS"},
        {"bitmcts", "ConnectFourBitMCTS"}
    };

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--mode" && i + 1 < argc) {
            mode = argv[++i];
        } else if (arg == "--algo1" && i + 1 < argc) {
            std::string algo_name = argv[++i];
            if (algorithms.find(algo_name) != algorithms.end()) {
                algo1 = algorithms[algo_name];
            } else {
                std::cout << "알 수 없는 알고리즘: " << algo_name << std::endl;
                std::cout << "사용 가능한 알고리즘: ";
                for (const auto& pair : algorithms) {
                    std::cout << pair.first << " ";
                }
                std::cout << std::endl;
                return 1;
            }
        } else if (arg == "--algo2" && i + 1 < argc) {
            std::string algo_name = argv[++i];
            if (algorithms.find(algo_name) != algorithms.end()) {
                algo2 = algorithms[algo_name];
            } else {
                std::cout << "알 수 없는 알고리즘: " << algo_name << std::endl;
                std::cout << "사용 가능한 알고리즘: ";
                for (const auto& pair : algorithms) {
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
            std::cout << "사용법: connect_four_demo [옵션]" << std::endl;
            std::cout << "옵션:" << std::endl;
            std::cout << "  --mode MODE    실행 모드 (play, benchmark, compare, human)" << std::endl;
            std::cout << "  --algo1 ALGO   첫 번째 알고리즘 (random, mcts, bitmcts)" << std::endl;
            std::cout << "  --algo2 ALGO   두 번째 알고리즘 (random, mcts, bitmcts)" << std::endl;
            std::cout << "  --games N      벤치마크 모드에서 실행할 게임 수" << std::endl;
            std::cout << "  --difficulty D  사람 vs AI 게임 난이도 (0: 쉬움, 1: 보통, 2: 어려움)" << std::endl;
            std::cout << "  --help         이 도움말 메시지 표시" << std::endl;
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
        std::cout << "알 수 없는 모드: " << mode << std::endl;
        std::cout << "사용 가능한 모드: play, benchmark, compare" << std::endl;
        return 1;
    }
    
    return 0;
}