#ifndef CONNECT_FOUR_BASE_H
#define CONNECT_FOUR_BASE_H

#include "../base_algorithm.h"
#include "../../games/connect_four/connect_four_state.h"
#include <stdexcept>

// Connect Four 게임을 위한 기본 알고리즘 클래스
class ConnectFourBaseAlgorithm : public BaseAlgorithm {
public:
    ConnectFourBaseAlgorithm(const std::string& name) : BaseAlgorithm(name) {}
    
    // 기본 runAndEvaluate 구현 (Connect Four 게임에 적용)
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto game_state = dynamic_cast<const ConnectFourState*>(&state);
        if (!game_state) {
            throw std::invalid_argument("ConnectFourBaseAlgorithm: Invalid state type");
        }
        
        auto next_state = std::make_unique<ConnectFourState>(*game_state);
        next_state->progress(action);
        return next_state;
    }
};

#endif // CONNECT_FOUR_BASE_H 