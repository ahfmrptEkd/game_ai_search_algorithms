#include "../algorithms/algorithm_interface.h"
#include "../games/wallmaze/wallmaze_state.h"
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
#include <queue>

int randomAction(const WallMazeState& state) {
    auto legal_actions = state.legalActions();
    if (legal_actions.empty()) return -1;
    return legal_actions[GameUtil::mt_for_action() % legal_actions.size()];
}

int greedyAction(const WallMazeState& state) {
    auto legal_actions = state.legalActions();
    if (legal_actions.empty()) return -1;
    
    int best_action = -1;
    int best_score = -1;
    
    for (const auto& action : legal_actions) {
        WallMazeState next_state = state;
        next_state.progress(action);
        if (next_state.game_score_ > best_score) {
            best_score = next_state.game_score_;
            best_action = action;
        }
    }
    
    return best_action != -1 ? best_action : randomAction(state);
}

int beamSearchAction(const WallMazeState& state, int beam_width, int beam_depth) {
    std::priority_queue<WallMazeState> now_beam;
    WallMazeState best_state;

    now_beam.push(state);
    for (int t = 0; t < beam_depth; t++) {
        std::priority_queue<WallMazeState> next_beam;
        for (int i = 0; i < beam_width; i++) {
            if (now_beam.empty()) break;
            
            WallMazeState now_state = now_beam.top();
            now_beam.pop();
            
            for (const auto& action : now_state.legalActions()) {
                WallMazeState next_state = now_state;
                next_state.progress(action);
                next_state.evaluateScore();
                
                if (t == 0) {
                    next_state.first_action_ = action;
                }
                
                next_beam.push(next_state);
            }
        }
        
        now_beam = next_beam;
        if (!now_beam.empty()) {
            best_state = now_beam.top();
        }
        
        if (best_state.isDone()) {
            break;
        }
    }
    
    return best_state.first_action_ != -1 ? best_state.first_action_ : randomAction(state);
}

// 알고리즘 성능 결과 구조체
struct AlgorithmPerformance {
    double avg_score = 0.0;
    double avg_time_ms = 0.0;
    double min_score = 999999;
    double max_score = 0;
    double std_dev = 0.0;
    std::vector<double> scores;
};

AlgorithmPerformance testAlgorithmPerformance(
    const std::string& name,
    std::function<int(const WallMazeState&)> algorithm,
    int test_count,
    bool verbose = true
) {
    AlgorithmPerformance result;
    result.scores.reserve(test_count);
    double total_score = 0;
    double total_time = 0;
    
    if (verbose) {
        std::cout << name << " 알고리즘 " << test_count << "회 테스트 중..." << std::endl;
    }
    
    for (int i = 0; i < test_count; i++) {
        auto state = WallMazeState(i);
        
        auto start_time = std::chrono::high_resolution_clock::now();
        
        while (!state.isDone()) {
            int action = algorithm(state);
            state.progress(action);
        }
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        
        double score = state.game_score_;
        total_score += score;
        total_time += duration;
        result.scores.push_back(score);
        
        result.min_score = std::min(result.min_score, score);
        result.max_score = std::max(result.max_score, score);
        
        if (verbose && (i + 1) % 10 == 0) {
            std::cout << "  진행: " << i + 1 << "/" << test_count << std::endl;
        }
    }
    
    result.avg_score = total_score / test_count;
    result.avg_time_ms = total_time / test_count;
    
    double sum_squared_diff = 0;
    for (double score : result.scores) {
        double diff = score - result.avg_score;
        sum_squared_diff += diff * diff;
    }
    result.std_dev = std::sqrt(sum_squared_diff / test_count);
    
    if (verbose) {
        std::cout << "  완료: 평균 점수 " << result.avg_score 
                  << ", 평균 실행 시간 " << result.avg_time_ms << "ms" << std::endl;
    }
    
    return result;
}

// 빔 서치 매개변수 최적화 벤치마크
void beamParameterBenchmark(int test_count) {
    std::cout << "\n===== 빔 서치 매개변수 최적화 =====" << std::endl;
    
    std::vector<int> beam_widths = {10, 50, 100, 200};
    std::vector<int> beam_depths = {5, 10, 15, 20};
    
    std::cout << std::left << std::setw(10) << "Width" << std::setw(10) << "Depth" 
              << std::setw(15) << "Avg Score" << std::setw(15) << "Avg Time (ms)" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    
    for (int width : beam_widths) {
        for (int depth : beam_depths) {
            auto algorithm = [width, depth](const WallMazeState& state) {
                return beamSearchAction(state, width, depth);
            };
            
            auto result = testAlgorithmPerformance("Beam " + std::to_string(width) + "x" + std::to_string(depth), 
                                                  algorithm, test_count, false);
            
            std::cout << std::left << std::setw(10) << width << std::setw(10) << depth
                      << std::fixed << std::setprecision(2) << std::setw(15) << result.avg_score
                      << std::setw(15) << result.avg_time_ms << std::endl;
        }
    }
}

void evaluationFunctionTest(int test_count) {
    std::cout << "\n===== 평가 함수 비교 테스트 =====" << std::endl;
    
    // 기존 평가 함수 (현재 점수만 고려)
    auto basic_evaluation = [](const WallMazeState& state) {
        auto legal_actions = state.legalActions();
        if (legal_actions.empty()) return -1;
        
        int best_action = -1;
        int best_score = -1;
        
        for (const auto& action : legal_actions) {
            WallMazeState next_state = state;
            next_state.progress(action);
            if (next_state.game_score_ > best_score) {
                best_score = next_state.game_score_;
                best_action = action;
            }
        }
        
        return best_action != -1 ? best_action : randomAction(state);
    };
    
    auto enhanced_evaluation = [](const WallMazeState& state) {
        auto legal_actions = state.legalActions();
        if (legal_actions.empty()) return -1;
        
        int best_action = -1;
        ScoreType best_score = -GameConstants::INF;
        
        for (const auto& action : legal_actions) {
            WallMazeState next_state = state;
            next_state.progress(action);
            ScoreType score = next_state.evaluateScore();
            
            if (score > best_score) {
                best_score = score;
                best_action = action;
            }
        }
        
        return best_action != -1 ? best_action : randomAction(state);
    };
    
    auto basic_result = testAlgorithmPerformance("기본 평가 함수", basic_evaluation, test_count);
    auto enhanced_result = testAlgorithmPerformance("향상된 BFS 평가 함수", enhanced_evaluation, test_count);
    
    std::cout << "\n평가 함수 성능 비교 (테스트 횟수: " << test_count << ")" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    std::cout << std::left << std::setw(20) << "평가 함수" 
              << std::setw(15) << "평균 점수" 
              << std::setw(15) << "최소 점수"
              << std::setw(15) << "최대 점수"
              << std::setw(15) << "표준 편차" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    std::cout << std::left << std::setw(20) << "기본 평가 함수" 
              << std::fixed << std::setprecision(2) << std::setw(15) << basic_result.avg_score
              << std::setw(15) << basic_result.min_score
              << std::setw(15) << basic_result.max_score
              << std::setw(15) << basic_result.std_dev << std::endl;
              
    std::cout << std::left << std::setw(20) << "향상된 BFS 평가 함수" 
              << std::fixed << std::setprecision(2) << std::setw(15) << enhanced_result.avg_score
              << std::setw(15) << enhanced_result.min_score
              << std::setw(15) << enhanced_result.max_score
              << std::setw(15) << enhanced_result.std_dev << std::endl;
              
    double improvement = ((enhanced_result.avg_score - basic_result.avg_score) / basic_result.avg_score) * 100;
    std::cout << "\nBFS 평가 함수 성능 향상: " << std::fixed << std::setprecision(2) << improvement << "%" << std::endl;
}

int main(int argc, char* argv[]) {
    GameUtil::mt_for_action.seed(time(nullptr));
    
    std::string benchmark_mode = "all";
    int test_count = 50;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--mode" && i + 1 < argc) {
            benchmark_mode = argv[++i];
        } else if (arg == "--tests" && i + 1 < argc) {
            test_count = std::stoi(argv[++i]);
        } else if (arg == "--help") {
            std::cout << "사용법: wallmaze_benchmark [옵션]" << std::endl
                      << "옵션:" << std::endl
                      << "  --mode MODE      벤치마크 모드 (all, algorithms, beams, evaluation)" << std::endl
                      << "  --tests N        각 테스트 당 실행 횟수 (기본값: 50)" << std::endl
                      << "  --help           이 도움말 메시지 표시" << std::endl;
            return 0;
        }
    }
    
    std::cout << "===== WallMaze 알고리즘 벤치마크 =====" << std::endl;
    std::cout << "각 테스트 " << test_count << "회 실행" << std::endl;
    
    std::map<std::string, std::function<int(const WallMazeState&)>> algorithms = {
        {"Random", randomAction},
        {"Greedy", greedyAction},
        {"Beam Search", [](const WallMazeState& state) {
            return beamSearchAction(state, 100, 10);    
        }}
    };
    
    if (benchmark_mode == "all" || benchmark_mode == "algorithms") {
        std::cout << "\n===== 알고리즘 성능 비교 =====" << std::endl;
        
        std::map<std::string, AlgorithmPerformance> results;
        
        for (const auto& [name, algorithm] : algorithms) {
            results[name] = testAlgorithmPerformance(name, algorithm, test_count);
        }
        
        std::cout << "\n알고리즘 성능 비교 결과" << std::endl;
        std::cout << std::string(65, '-') << std::endl;
        std::cout << std::left << std::setw(15) << "알고리즘" 
                  << std::setw(15) << "평균 점수" 
                  << std::setw(15) << "평균 시간(ms)"
                  << std::setw(20) << "점수 범위" << std::endl;
        std::cout << std::string(65, '-') << std::endl;
        
        for (const auto& [name, result] : results) {
            std::cout << std::left << std::setw(15) << name 
                      << std::fixed << std::setprecision(2) << std::setw(15) << result.avg_score
                      << std::setw(15) << result.avg_time_ms
                      << std::setw(20) << result.min_score << "-" << result.max_score << std::endl;
        }
    }
    
    if (benchmark_mode == "all" || benchmark_mode == "beams") {
        // 빔 서치 매개변수 최적화
        beamParameterBenchmark(test_count / 2);  // 조합이 많으므로 테스트 수 감소
    }
    
    if (benchmark_mode == "all" || benchmark_mode == "evaluation") {
        evaluationFunctionTest(test_count);
    }
    
    return 0;
}