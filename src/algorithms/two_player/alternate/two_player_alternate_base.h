#ifndef TWO_PLAYER_ALTERNATE_BASE_H
#define TWO_PLAYER_ALTERNATE_BASE_H

#include "../../base_algorithm.h"
#include "../../../games/twomaze/twomaze_state.h"
#include <stdexcept>

// 두 플레이어 교대 게임을 위한 기본 알고리즘 클래스
class TwoPlayerAlternateBaseAlgorithm : public BaseAlgorithm {
public:
    TwoPlayerAlternateBaseAlgorithm(const std::string& name) : BaseAlgorithm(name) {}
    
    // 기본 runAndEvaluate 구현 (대부분의 두 플레이어 교대 게임에 적용 가능)
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto game_state = dynamic_cast<const TwoMazeState*>(&state);
        if (!game_state) {
            throw std::invalid_argument("TwoPlayerAlternateBaseAlgorithm: Invalid state type");
        }
        
        auto next_state = std::make_unique<TwoMazeState>(*game_state);
        next_state->progress(action);
        return next_state;
    }
};

#endif // TWO_PLAYER_ALTERNATE_BASE_H 