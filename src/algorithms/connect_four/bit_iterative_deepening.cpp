#include "bit_iterative_deepening.h"
#include <random>

ConnectFourBitIterativeDeepeningAlgorithm::ConnectFourBitIterativeDeepeningAlgorithm() : random_engine_(std::random_device{}()) {}

// 알파베타 알고리즘 구현 (비트보드 버전)
std::pair<int, double> ConnectFourBitIterativeDeepeningAlgorithm::alphabeta(
    const ConnectFourBitBoardState& state, int depth, double alpha, double beta, bool maximizing_player) {
    
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
        
        // 휴리스틱 평가 - 중앙 컬럼에 가중치 부여
        double score = 0.0;
        for (int col = 0; col < GameConstants::ConnectFour::W; col++) {
            double weight = 1.0;
            // 중앙에 가까울수록 가중치 증가
            weight = 1.0 + 0.3 * (3 - abs(col - 3));
            if (state.isValidMove(col)) {
                score += weight * 0.1;
            }
        }
        
        // 플레이어에 따라 부호 변경
        return {-1, maximizing_player ? score : -score};
    }
    
    std::vector<int> legal_actions = state.legalActions();
    if (legal_actions.empty()) {
        return {-1, 0.0};
    }
    
    int best_action = legal_actions[0];
    double best_value = maximizing_player ? -std::numeric_limits<double>::infinity() : std::numeric_limits<double>::infinity();
    
    for (int action : legal_actions) {
        ConnectFourBitBoardState next_state = state;
        next_state.advance(action);
        
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

// Iterative Deepening 구현 (비트보드 버전)
int ConnectFourBitIterativeDeepeningAlgorithm::iterativeDeepening(const ConnectFourState& state) {
    // 일반 게임 상태를 비트보드 상태로 변환
    ConnectFourBitBoardState bit_state(state);
    
    int best_action = -1;
    
    // 가능한 모든 행동 확인
    std::vector<int> legal_actions = bit_state.legalActions();
    if (legal_actions.empty()) {
        return -1;
    }
    
    // 적어도 하나의 유효한 수는 반환
    best_action = legal_actions[random_engine_() % legal_actions.size()];
    
    auto start_time = std::chrono::high_resolution_clock::now();
    int max_depth = params_.searchDepth;
    
    // 시간 제한이 0이면 설정된 깊이까지만 탐색
    if (params_.timeThreshold <= 0) {
        return alphabeta(bit_state, max_depth, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), true).first;
    }
    
    // 시간 제한 내에서 점진적으로 깊이 증가
    for (int depth = 1; depth <= max_depth; depth++) {
        int current_action = alphabeta(bit_state, depth, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), true).first;
        
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

int ConnectFourBitIterativeDeepeningAlgorithm::selectAction(const GameState& state) {
    auto connectfour_state = static_cast<const ConnectFourState&>(state);
    return iterativeDeepening(connectfour_state);
}

std::string ConnectFourBitIterativeDeepeningAlgorithm::getName() const {
    return "Bit Iterative Deepening (Connect Four)";
}

void ConnectFourBitIterativeDeepeningAlgorithm::setParams(const AlgorithmParams& params) {
    params_ = params;
}

std::unique_ptr<GameState> ConnectFourBitIterativeDeepeningAlgorithm::runAndEvaluate(const GameState& state, int action) {
    auto connectfour_state = static_cast<const ConnectFourState&>(state);
    auto next_state = std::make_unique<ConnectFourState>(connectfour_state);
    next_state->progress(action);
    return next_state;
} 