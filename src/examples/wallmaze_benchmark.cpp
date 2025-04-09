#include "../algorithms/algorithm_interface.h"
#include "../games/wallmaze/wallmaze_state.h"
#include "../games/wallmaze/zobrist_hash.h"
#include "../algorithms/pathfinding/pathfinding.h"
#include "../common/coord.h"
#include "../common/game_util.h"
#include "../games/wallmaze/wallmaze_bitset_state.h"
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
#include <unordered_set>

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

int beamSearchAction(const WallMazeState& state, int beam_width, int beam_depth, bool use_hash = true) {
    std::priority_queue<WallMazeState> now_beam;
    WallMazeState best_state;
    std::unordered_set<uint64_t> hash_check;
    
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
                
                // 해시 사용 시 중복 상태 체크
                if (use_hash && t >= 1 && hash_check.count(next_state.hash_) > 0) {
                    continue;
                }
                
                if (use_hash) {
                    hash_check.insert(next_state.hash_);
                }
                
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

int beamSearchActionBitset(const WallMazeBitSetState& state, int beam_width, int beam_depth);

int bitsetBeamSearchAction(const WallMazeState& state, int beam_width, int beam_depth, bool use_hash) {
    WallMazeBitSetState bitset_state(state);
    return beamSearchActionBitset(bitset_state, beam_width, beam_depth);
}

// 경로 찾기 알고리즘 기반 행동 선택 (가장 가까운 점수 위치로 이동)
int pathfindingAction(const WallMazeState& state, PathfindingConstants::Algorithm algo)
{
    // 가장 가까운 점수 위치 찾기
    Coord nearestPoint = state.findNearestPoint(algo);
    
    // 적절한 점수 위치를 찾지 못했다면 랜덤 행동
    if (nearestPoint.x_ == -1 || nearestPoint.y_ == -1) {
        return randomAction(state);
    }
    
    // 해당 위치로 이동하기 위한 다음 행동 반환
    int action = state.getNextActionTowards(nearestPoint, algo);
    
    // 경로를 찾지 못했거나 다음 행동이 없는 경우 랜덤 행동
    if (action == -1) {
        return randomAction(state);
    }
    
    return action;
}

// 가치 기반 경로 찾기 (점수/거리 비율 최적화)
int valueBasedPathfindingAction(const WallMazeState& state, PathfindingConstants::Algorithm algo)
{
    // 가치가 가장 높은 점수 위치 찾기
    Coord bestPoint = state.findHighestValuePoint(algo);
    
    // 적절한 점수 위치를 찾지 못했다면 랜덤 행동
    if (bestPoint.x_ == -1 || bestPoint.y_ == -1) {
        return randomAction(state);
    }
    
    // 해당 위치로 이동하기 위한 다음 행동 반환
    int action = state.getNextActionTowards(bestPoint, algo);
    
    // 경로를 찾지 못했거나 다음 행동이 없는 경우 랜덤 행동
    if (action == -1) {
        return randomAction(state);
    }
    
    return action;
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

// 빔 서치 매개변수 최적화 벤치마크 - 기존 벤치마크 
void beamParameterBenchmark(int test_count) {
    std::cout << "\n===== 빔 서치 매개변수 최적화 =====" << std::endl;
    
    std::vector<int> beam_widths = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> beam_depths = {1, 2, 3, 4, 5, 6, 7, 8};
    
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

void hashEffectBenchmark(int test_count) {
    std::cout << "\n===== 해시 효과 벤치마크 =====" << std::endl;
    
    auto withHash = [](const WallMazeState& state) {
        return beamSearchAction(state, GameConstants::AlgorithmParams::SEARCH_WIDTH, GameConstants::AlgorithmParams::SEARCH_DEPTH, true);
    };
    
    auto withoutHash = [](const WallMazeState& state) {
        return beamSearchAction(state, GameConstants::AlgorithmParams::SEARCH_WIDTH, GameConstants::AlgorithmParams::SEARCH_DEPTH, false);
    };
    
    auto withHashResult = testAlgorithmPerformance("Beam Search + Hash", withHash, test_count);
    auto withoutHashResult = testAlgorithmPerformance("Beam Search No Hash", withoutHash, test_count);
    
    std::cout << "\n해시 사용 효과 (빔 서치 3x7):" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    std::cout << std::left << std::setw(20) << "버전" 
              << std::setw(15) << "평균 점수" 
              << std::setw(15) << "평균 시간(ms)" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    std::cout << std::left << std::setw(20) << "해시 미사용" 
              << std::fixed << std::setprecision(2) << std::setw(15) << withoutHashResult.avg_score
              << std::setw(15) << withoutHashResult.avg_time_ms << std::endl;
    
    std::cout << std::left << std::setw(20) << "해시 사용" 
              << std::fixed << std::setprecision(2) << std::setw(15) << withHashResult.avg_score
              << std::setw(15) << withHashResult.avg_time_ms << std::endl;
    
    double timeImprovement = ((withoutHashResult.avg_time_ms - withHashResult.avg_time_ms) / withoutHashResult.avg_time_ms) * 100;
    
    std::cout << "\n시간 단축률: " << std::fixed << std::setprecision(2) << timeImprovement << "%" << std::endl;
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

// 경로 탐색 알고리즘 성능 비교
void pathfindingBenchmark(int test_count) {
    std::cout << "\n===== 경로 탐색 알고리즘 성능 비교 =====" << std::endl;
    
    std::map<std::string, std::function<int(const WallMazeState&)>> pathAlgorithms = {
        {"BFS", [](const WallMazeState& state) {
            return pathfindingAction(state, PathfindingConstants::Algorithm::BFS);
        }},
        {"DFS", [](const WallMazeState& state) {
            return pathfindingAction(state, PathfindingConstants::Algorithm::DFS);
        }},
        {"A*", [](const WallMazeState& state) {
            return pathfindingAction(state, PathfindingConstants::Algorithm::ASTAR);
        }},
        {"Dijkstra", [](const WallMazeState& state) {
            return pathfindingAction(state, PathfindingConstants::Algorithm::DIJKSTRA);
        }},
        {"Value-Based A*", [](const WallMazeState& state) {
            return valueBasedPathfindingAction(state, PathfindingConstants::Algorithm::ASTAR);
        }}
    };
    
    std::map<std::string, AlgorithmPerformance> results;
    
    for (const auto& [name, algorithm] : pathAlgorithms) {
        results[name] = testAlgorithmPerformance(name, algorithm, test_count);
    }
    
    std::cout << "\n경로 탐색 알고리즘 성능 비교 결과" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    std::cout << std::left << std::setw(20) << "알고리즘" 
              << std::setw(15) << "평균 점수" 
              << std::setw(15) << "평균 시간(ms)"
              << std::setw(15) << "최소 점수"
              << std::setw(15) << "최대 점수" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    for (const auto& [name, result] : results) {
        std::cout << std::left << std::setw(20) << name 
                  << std::fixed << std::setprecision(2) << std::setw(15) << result.avg_score
                  << std::setw(15) << result.avg_time_ms
                  << std::setw(15) << result.min_score
                  << std::setw(15) << result.max_score << std::endl;
    }
    
    std::string best_score_algo;
    double best_score = -1;
    
    std::string fastest_algo;
    double fastest_time = std::numeric_limits<double>::max();
    
    for (const auto& [name, result] : results) {
        if (result.avg_score > best_score) {
            best_score = result.avg_score;
            best_score_algo = name;
        }
        
        if (result.avg_time_ms < fastest_time) {
            fastest_time = result.avg_time_ms;
            fastest_algo = name;
        }
    }
    
    std::cout << "\n분석:" << std::endl;
    std::cout << "가장 높은 점수 달성 알고리즘: " << best_score_algo << " (평균 " << best_score << "점)" << std::endl;
    std::cout << "가장 빠른 실행 시간 알고리즘: " << fastest_algo << " (평균 " << fastest_time << "ms)" << std::endl;
    
    auto& astar_result = results["A*"];
    auto& bfs_result = results["BFS"];
    
    double score_diff = ((astar_result.avg_score - bfs_result.avg_score) / bfs_result.avg_score) * 100;
    double time_diff = ((bfs_result.avg_time_ms - astar_result.avg_time_ms) / bfs_result.avg_time_ms) * 100;
    
    std::cout << "A* vs BFS 비교: " << std::endl;
    std::cout << "  - 점수 차이: " << std::fixed << std::setprecision(2) << score_diff << "%" << std::endl;
    std::cout << "  - 시간 차이: " << std::fixed << std::setprecision(2) << time_diff << "%" << std::endl;
    
    auto& value_astar_result = results["Value-Based A*"];
    
    double value_score_diff = ((value_astar_result.avg_score - astar_result.avg_score) / astar_result.avg_score) * 100;
    double value_time_diff = ((astar_result.avg_time_ms - value_astar_result.avg_time_ms) / astar_result.avg_time_ms) * 100;
    
    std::cout << "가치 기반 A* vs 일반 A* 비교: " << std::endl;
    std::cout << "  - 점수 차이: " << std::fixed << std::setprecision(2) << value_score_diff << "%" << std::endl;
    std::cout << "  - 시간 차이: " << std::fixed << std::setprecision(2) << value_time_diff << "%" << std::endl;
}

// 모든 알고리즘 통합 비교
void compareAllAlgorithms(int test_count) {
    std::cout << "\n===== 모든 알고리즘 통합 비교 =====" << std::endl;
    
    // Zobrist 해시 초기화
    zobrist_hash::init();
    
    std::map<std::string, std::function<int(const WallMazeState&)>> algorithms = {
        {"Random", randomAction},
        {"Greedy", greedyAction},
        {"Beam Search", [](const WallMazeState& state) {
            return beamSearchAction(state, GameConstants::AlgorithmParams::SEARCH_WIDTH, GameConstants::AlgorithmParams::SEARCH_DEPTH, true);
        }},
        {"BFS Pathfinding", [](const WallMazeState& state) {
            return pathfindingAction(state, PathfindingConstants::Algorithm::BFS);
        }},
        {"DFS Pathfinding", [](const WallMazeState& state) {
            return pathfindingAction(state, PathfindingConstants::Algorithm::DFS);
        }},
        {"A* Pathfinding", [](const WallMazeState& state) {
            return pathfindingAction(state, PathfindingConstants::Algorithm::ASTAR);
        }},
        {"Dijkstra Pathfinding", [](const WallMazeState& state) {
            return pathfindingAction(state, PathfindingConstants::Algorithm::DIJKSTRA);
        }},
        {"Value-Based A*", [](const WallMazeState& state) {
            return valueBasedPathfindingAction(state, PathfindingConstants::Algorithm::ASTAR);
        }}
    };
    
    std::map<std::string, AlgorithmPerformance> results;
    
    // 각 알고리즘 성능 테스트
    for (const auto& [name, algorithm] : algorithms) {
        results[name] = testAlgorithmPerformance(name, algorithm, test_count);
    }
    
    std::vector<std::pair<std::string, AlgorithmPerformance>> sorted_results(results.begin(), results.end());
    
    std::sort(sorted_results.begin(), sorted_results.end(), 
             [](const auto& a, const auto& b) {
                 return a.second.avg_score > b.second.avg_score;
             });
    
    std::cout << "\n=== 알고리즘 성능 순위 (평균 점수 기준) ===" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    std::cout << std::left << std::setw(5) << "순위" 
              << std::setw(20) << "알고리즘" 
              << std::setw(15) << "평균 점수" 
              << std::setw(15) << "평균 시간(ms)"
              << std::setw(15) << "점수 효율*" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    for (size_t i = 0; i < sorted_results.size(); i++) {
        const auto& [name, result] = sorted_results[i];
        
        // 점수 효율 = 점수 / 시간 (점수 당 소요된 시간의 역수, 높을수록 좋음)
        double score_efficiency = result.avg_time_ms > 0 ? result.avg_score / result.avg_time_ms : 0;
        
        std::cout << std::left << std::setw(5) << (i + 1)
                  << std::setw(20) << name 
                  << std::fixed << std::setprecision(2) << std::setw(15) << result.avg_score
                  << std::setw(15) << result.avg_time_ms
                  << std::setw(15) << score_efficiency << std::endl;
    }
    
    std::cout << "\n* 점수 효율 = 평균 점수 / 평균 시간 (높을수록 효율적)" << std::endl;
    
    std::string most_efficient_algo;
    double best_efficiency = -1;
    
    for (const auto& [name, result] : results) {
        double efficiency = result.avg_time_ms > 0 ? result.avg_score / result.avg_time_ms : 0;
        if (efficiency > best_efficiency) {
            best_efficiency = efficiency;
            most_efficient_algo = name;
        }
    }
    
    std::cout << "\n가장 효율적인 알고리즘: " << most_efficient_algo 
              << " (점수 효율: " << std::fixed << std::setprecision(2) << best_efficiency << ")" << std::endl;
}

void bitsetOptimizationBenchmark(int test_count) {
    std::cout << "\n===== 비트셋 최적화 성능 비교 =====" << std::endl;
    
    // 일반 구현 빔 서치
    auto standardImpl = [](const WallMazeState& state) {
        return beamSearchAction(state, GameConstants::AlgorithmParams::SEARCH_WIDTH, GameConstants::AlgorithmParams::SEARCH_DEPTH, true);
    };
    
    // 비트셋 구현 빔 서치
    auto bitsetImpl = [](const WallMazeState& state) {
        return bitsetBeamSearchAction(state, GameConstants::AlgorithmParams::SEARCH_WIDTH, GameConstants::AlgorithmParams::SEARCH_DEPTH, true);
    };
    
    auto standard_result = testAlgorithmPerformance("표준 구현", standardImpl, test_count);
    auto bitset_result = testAlgorithmPerformance("비트셋 최적화", bitsetImpl, test_count);
    
    std::cout << "\n===== 비트셋 최적화 결과 =====\n";
    std::cout << std::string(60, '-') << std::endl;
    std::cout << std::left << std::setw(20) << "구현 방식" 
              << std::setw(15) << "평균 점수" 
              << std::setw(15) << "평균 시간(ms)" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    std::cout << std::left << std::setw(20) << "표준 구현" 
              << std::fixed << std::setprecision(2) << std::setw(15) << standard_result.avg_score
              << std::setw(15) << standard_result.avg_time_ms << std::endl;
    
    std::cout << std::left << std::setw(20) << "비트셋 최적화" 
              << std::fixed << std::setprecision(2) << std::setw(15) << bitset_result.avg_score
              << std::setw(15) << bitset_result.avg_time_ms << std::endl;
    
    // 성능 향상율 계산
    double time_improvement = ((standard_result.avg_time_ms - bitset_result.avg_time_ms) / standard_result.avg_time_ms) * 100;
    
    std::cout << "\n===== 성능 향상 분석 =====\n";
    std::cout << "실행 시간 단축: " << std::fixed << std::setprecision(2) << time_improvement << "%" << std::endl;
    
    // 거리 계산 알고리즘 개별 성능 측정
    std::cout << "\n===== 최근접 점수 거리 계산 성능 비교 =====" << std::endl;
    
    auto standard_distance = [test_count]() {
        double total_time = 0;
        auto seed = time(nullptr);
        std::mt19937 mt(seed);
        
        for (int i = 0; i < test_count; i++) {
            auto state = WallMazeState(mt());
            
            auto start_time = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < 100; j++) {
                state.getDistanceToNearestPoint();
            }
            auto end_time = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                end_time - start_time).count() / 100.0;
            
            total_time += duration;
        }
        
        return total_time / test_count;
    };
    
    auto bitset_distance = [test_count]() {
        double total_time = 0;
        auto seed = time(nullptr);
        std::mt19937 mt(seed);
        
        for (int i = 0; i < test_count; i++) {
            auto state = WallMazeState(mt());
            auto bitset_state = WallMazeBitSetState(state);
            
            auto start_time = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < 100; j++) {
                bitset_state.getDistanceToNearestPoint();
            }
            auto end_time = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                end_time - start_time).count() / 100.0;
            
            total_time += duration;
        }
        
        return total_time / test_count;
    };
    
    double std_time = standard_distance();
    double bit_time = bitset_distance();
    double dist_improvement = ((std_time - bit_time) / std_time) * 100;
    
    std::cout << "표준 거리계산 시간: " << std::fixed << std::setprecision(2) << std_time << "μs" << std::endl;
    std::cout << "비트셋 거리계산 시간: " << std::fixed << std::setprecision(2) << bit_time << "μs" << std::endl;
    std::cout << "거리계산 속도 향상: " << std::fixed << std::setprecision(2) << dist_improvement << "%" << std::endl;
}

int main(int argc, char* argv[]) {
    GameUtil::mt_for_action.seed(time(nullptr));
    
    // Zobrist 해시 초기화
    zobrist_hash::init();
    
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
                      << "  --mode MODE      벤치마크 모드 (all, algorithms, beams, evaluation, hash, pathfinding, compare, bitset)" << std::endl
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
            return beamSearchAction(state, GameConstants::AlgorithmParams::SEARCH_WIDTH, GameConstants::AlgorithmParams::SEARCH_DEPTH);    
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
    
    if (benchmark_mode == "all" || benchmark_mode == "hash") {
        hashEffectBenchmark(test_count);
    }
    
    if (benchmark_mode == "all" || benchmark_mode == "pathfinding") {
        pathfindingBenchmark(test_count);
    }

    if (benchmark_mode == "all" || benchmark_mode == "bitset") {
    bitsetOptimizationBenchmark(test_count);
    }
    
    if (benchmark_mode == "compare") {
        compareAllAlgorithms(test_count);
    }

    return 0;
}