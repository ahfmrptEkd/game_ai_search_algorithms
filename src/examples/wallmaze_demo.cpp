#include "../algorithms/algorithm_interface.h"
#include "../games/wallmaze/wallmaze_state.h"
#include "../common/coord.h"
#include "../common/game_util.h"
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <functional>
#include <chrono>
#include <iomanip>
#include <queue>

int randomAction(const WallMazeState& state)
{
    auto legal_actions = state.legalActions();
    if (legal_actions.empty()) {
        return -1;
    }
    return legal_actions[GameUtil::mt_for_action() % (legal_actions.size())];
}

int greedyAction(const WallMazeState& state)
{
    auto legal_actions = state.legalActions();
    if (legal_actions.empty()) {
        return -1;
    }
    
    int best_action = -1;
    int best_score = -1;
    
    for (const auto& action : legal_actions) {
        WallMazeState next_state = state;
        next_state.progress(action);
        
        // 단순히 현재 게임 점수만 평가
        int score = next_state.game_score_;
        if (score > best_score) {
            best_score = score;
            best_action = action;
        }
    }
    
    if (best_action == -1) {
        best_action = legal_actions[GameUtil::mt_for_action() % legal_actions.size()];
    }
    
    return best_action;
}

// 빔 탐색 알고리즘 (향상된 BFS 평가 함수 사용)
int beamSearchAction(const WallMazeState& state, int beam_width, int search_depth)
{
    std::priority_queue<WallMazeState> now_beam;
    WallMazeState best_state;
    
    now_beam.push(state);
    for (int t = 0; t < search_depth; t++) {
        std::priority_queue<WallMazeState> next_beam;
        for (int i = 0; i < beam_width; i++) {
            if (now_beam.empty()) {
                break;
            }
            
            WallMazeState now_state = now_beam.top();
            now_beam.pop();
            
            for (const auto& action : now_state.legalActions()) {
                WallMazeState next_state = now_state;
                next_state.progress(action);
                next_state.evaluateScore();  // BFS 기반 향상된 평가 함수 사용
                
                if (t == 0) {
                    next_state.first_action_ = action;
                }
                
                next_beam.push(next_state);
            }
        }
        
        now_beam = next_beam;
        
        if (now_beam.empty()) {
            break;
        }
        
        best_state = now_beam.top();
        
        if (best_state.isDone()) {
            break;
        }
    }
    
    return best_state.first_action_ != -1 ? best_state.first_action_ : randomAction(state);
}

void playGameWithAlgorithm(const std::string& algorithm_name, int seed) {
    AlgorithmParams params;
    
    // 게임 설정
    params.searchWidth = 100;
    params.searchDepth = 10;
    
    std::function<int(const WallMazeState&)> algorithm;
    
    if (algorithm_name == "random") {
        algorithm = randomAction;
    } else if (algorithm_name == "greedy") {
        algorithm = greedyAction;
    } else if (algorithm_name == "beam") {
        algorithm = [&params](const WallMazeState& state) {
            return beamSearchAction(state, params.searchWidth, params.searchDepth);
        };
    } else {
        std::cout << "Unknown algorithm: " << algorithm_name << ". Using random instead." << std::endl;
        algorithm = randomAction;
    }
    
    auto state = WallMazeState(seed);
    
    std::cout << "Starting game with " << algorithm_name << " algorithm..." << std::endl;
    std::cout << state.toString() << std::endl;
    
    while (!state.isDone()) {
        int action = algorithm(state);
        state.progress(action);
        std::cout << state.toString() << std::endl;
    }
    
    std::cout << "Game finished! Final score: " << state.game_score_ << std::endl;
}

void benchmarkAlgorithms(int num_games) {
    std::map<std::string, std::function<int(const WallMazeState&)>> algorithms = {
        {"Random", randomAction},
        {"Greedy", greedyAction},
        {"Beam Search", [](const WallMazeState& state) {
            return beamSearchAction(state, 100, 10);
        }}
    };
    
    std::map<std::string, double> total_scores;
    std::map<std::string, double> total_times;
    
    std::cout << "벤치마크 테스트를 " << num_games << "게임씩 실행중..." << std::endl;
    
    for (int seed = 0; seed < num_games; seed++) {
        for (const auto& [name, algo] : algorithms) {
            auto state = WallMazeState(seed);
            
            auto start_time = std::chrono::high_resolution_clock::now();
            
            while (!state.isDone()) {
                int action = algo(state);
                state.progress(action);
            }
            
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
            
            total_scores[name] += state.game_score_;
            total_times[name] += duration;
        }
        
        if ((seed + 1) % 10 == 0 || seed == num_games - 1) {
            std::cout << "진행: " << (seed + 1) << "/" << num_games << " 게임 완료" << std::endl;
        }
    }
    
    std::cout << "\n===== 알고리즘 벤치마크 결과 =====" << std::endl;
    std::cout << std::left << std::setw(15) << "알고리즘" 
              << std::setw(15) << "평균 점수" 
              << std::setw(15) << "평균 실행시간" << std::endl;
    std::cout << std::string(45, '-') << std::endl;
    
    for (const auto& [name, score] : total_scores) {
        double avg_score = score / num_games;
        double avg_time = total_times[name] / num_games;
        
        std::cout << std::left << std::setw(15) << name 
                  << std::fixed << std::setprecision(2) << std::setw(15) << avg_score 
                  << std::setw(15) << avg_time << "ms" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    GameUtil::mt_for_action.seed(0);
    
    std::map<std::string, std::string> algorithms = {
        {"random", "Random"},
        {"greedy", "Greedy"},
        {"beam", "Beam Search"}
    };
    
    std::string mode = "play";
    std::string algorithm = "beam";
    int games = 50;
    int seed = GameUtil::mt_for_action();
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--mode" && i + 1 < argc) {
            mode = argv[++i];
        } else if (arg == "--algo" && i + 1 < argc) {
            algorithm = argv[++i];
        } else if (arg == "--games" && i + 1 < argc) {
            games = std::stoi(argv[++i]);
        } else if (arg == "--seed" && i + 1 < argc) {
            seed = std::stoi(argv[++i]);
        } else if (arg == "--help") {
            std::cout << "사용법: wallmaze_demo [옵션]" << std::endl
                      << "옵션:" << std::endl
                      << "  --mode MODE      실행 모드 (play 또는 benchmark)" << std::endl
                      << "  --algo ALGO      알고리즘 (random, greedy, beam)" << std::endl
                      << "  --games N        벤치마크 모드에서 실행할 게임 수" << std::endl
                      << "  --seed N         게임 초기화를 위한 시드" << std::endl
                      << "  --help           이 도움말 메시지 표시" << std::endl;
            return 0;
        }
    }
    
    if (mode == "play") {
        playGameWithAlgorithm(algorithm, seed);
    } else if (mode == "benchmark") {
        benchmarkAlgorithms(games);
    } else {
        std::cout << "알 수 없는 모드: " << mode << std::endl
                  << "유효한 모드: play, benchmark" << std::endl;
        return 1;
    }
    
    return 0;
}