#ifndef TWO_PLAYER_SIMULTANEOUS_BASE_H
#define TWO_PLAYER_SIMULTANEOUS_BASE_H

#include "../../base_algorithm.h"
#include "../../../games/simmaze/simmaze_state.h"
#include <stdexcept>

// 두 플레이어 동시 행동 게임을 위한 기본 알고리즘 클래스
class TwoPlayerSimultaneousBaseAlgorithm : public BaseAlgorithm {
public:
    TwoPlayerSimultaneousBaseAlgorithm(const std::string& name) : BaseAlgorithm(name) {}
    
    // 기본 runAndEvaluate 구현 (대부분의 두 플레이어 동시 행동 게임에 적용 가능)
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto game_state = dynamic_cast<const SimMazeState*>(&state);
        if (!game_state) {
            throw std::invalid_argument("TwoPlayerSimultaneousBaseAlgorithm: Invalid state type");
        }
        
        auto next_state = std::make_unique<SimMazeState>(*game_state);
        // 동시 게임에서는 자신의 액션만 설정하고, 상대방 액션은 외부에서 설정됨
        next_state->setAction(params_.playerId, action);
        return next_state;
    }
};

#endif // TWO_PLAYER_SIMULTANEOUS_BASE_H 