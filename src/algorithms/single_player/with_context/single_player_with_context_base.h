#ifndef SINGLE_PLAYER_WITH_CONTEXT_BASE_H
#define SINGLE_PLAYER_WITH_CONTEXT_BASE_H

#include "../../base_algorithm.h"
#include "../../../games/maze/maze_state.h"
#include <stdexcept>

// 컨텍스트 있는 단일 플레이어 게임을 위한 기본 알고리즘 클래스
class SinglePlayerWithContextBaseAlgorithm : public BaseAlgorithm {
public:
    SinglePlayerWithContextBaseAlgorithm(const std::string& name) : BaseAlgorithm(name) {}
    
    // 기본 runAndEvaluate 구현 (대부분의 단일 플레이어 게임에 적용 가능)
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto game_state = dynamic_cast<const MazeState*>(&state);
        if (!game_state) {
            throw std::invalid_argument("SinglePlayerWithContextBaseAlgorithm: Invalid state type");
        }
        
        auto next_state = std::make_unique<MazeState>(*game_state);
        next_state->progress(action);
        return next_state;
    }
};

#endif // SINGLE_PLAYER_WITH_CONTEXT_BASE_H 