#include "iterative_deepening.h"
#include <random>

ConnectFourIterativeDeepeningAlgorithm::ConnectFourIterativeDeepeningAlgorithm() : random_engine_(std::random_device{}()) {}

std::pair<int, double> ConnectFourIterativeDeepeningAlgorithm::alphabeta(
    const ConnectFourState& state, int depth, double alpha, double beta, bool maximizing_player) {
    
    // 게임이 종료되었거나 최대 깊이에 도달한 경우
    if (state.isDone() || depth == 0) {
        WinningStatus status = state.getWinningStatus();
        if (status == WinningStatus::WIN) {
            return {-1, maximizing_player ? 1.0 : -1.0};
        } else if (status == WinningStatus::LOSE) {
            return {-1, maximizing_player ? -1.0 : 1.0};
        } else if (status == WinningStatus::DRAW) {
            return {-1, 0.0};
        }
        
        // 평가 함수 사용 - const가 아니므로 복사본 생성
        ConnectFourState state_copy = state;
        return {-1, state_copy.evaluateScore() / 1000.0 * (maximizing_player ? 1.0 : -1.0)};
    }
    
    std::vector<int> legal_actions = state.legalActions();
    if (legal_actions.empty()) {
        return {-1, 0.0};
    }
    
    int best_action = legal_actions[0];
    double best_value = maximizing_player ? -std::numeric_limits<double>::infinity() : std::numeric_limits<double>::infinity();
    
    for (int action : legal_actions) {
        ConnectFourState next_state = ConnectFourState(state);
        next_state.progress(action);
        
        double value = alphabeta(next_state, depth - 1, alpha, beta, !maximizing_player).second;
        
        if (maximizing_player) {
            if (value > best_value) {
                best_value = value;
                best_action = action;
            }
            alpha = std::max(alpha, best_value);
        } else {
            if (value < best_value) {
                best_value = value;
                best_action = action;
            }
            beta = std::min(beta, best_value);
        }
        
        // 알파-베타 가지치기
        if (beta <= alpha) {
            break;
        }
    }
    
    return {best_action, best_value};
}

// Iterative Deepening 구현
int ConnectFourIterativeDeepeningAlgorithm::iterativeDeepening(const ConnectFourState& state) {
    int best_action = -1;
    
    // 가능한 모든 행동 확인
    std::vector<int> legal_actions = state.legalActions();
    if (legal_actions.empty()) {
        return -1;
    }
    
    // 적어도 하나의 유효한 수는 반환
    best_action = legal_actions[random_engine_() % legal_actions.size()];
    
    auto start_time = std::chrono::high_resolution_clock::now();
    int max_depth = params_.searchDepth;
    
    // 시간 제한이 0이면 설정된 깊이까지만 탐색
    if (params_.timeThreshold <= 0) {
        return alphabeta(state, max_depth, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), true).first;
    }
    
    // 시간 제한 내에서 점진적으로 깊이 증가
    for (int depth = 1; depth <= max_depth; depth++) {
        int current_action = alphabeta(state, depth, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), true).first;
        
        // 유효한 행동을 찾았으면 업데이트
        if (current_action != -1) {
            best_action = current_action;
        }
        
        // 시간 확인
        auto current_time = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
        
        // 시간 제한에 근접했으면 현재까지의 최선의 수 반환
        if (elapsed >= params_.timeThreshold * 0.8) {
            break;
        }
    }
    
    return best_action;
}

int ConnectFourIterativeDeepeningAlgorithm::selectAction(const GameState& state) {
    auto connectfour_state = static_cast<const ConnectFourState&>(state);
    return iterativeDeepening(connectfour_state);
}

std::string ConnectFourIterativeDeepeningAlgorithm::getName() const {
    return "Iterative Deepening (Connect Four)";
}

void ConnectFourIterativeDeepeningAlgorithm::setParams(const AlgorithmParams& params) {
    params_ = params;
}

std::unique_ptr<GameState> ConnectFourIterativeDeepeningAlgorithm::runAndEvaluate(const GameState& state, int action) {
    auto connectfour_state = static_cast<const ConnectFourState&>(state);
    auto next_state = std::make_unique<ConnectFourState>(connectfour_state);
    next_state->progress(action);
    return next_state;
} 