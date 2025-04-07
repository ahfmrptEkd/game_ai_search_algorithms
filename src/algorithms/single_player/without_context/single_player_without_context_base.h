#ifndef SINGLE_PLAYER_WITHOUT_CONTEXT_BASE_H
#define SINGLE_PLAYER_WITHOUT_CONTEXT_BASE_H

#include "../../base_algorithm.h"
#include "../../../games/automaze/automaze_state.h"
#include <stdexcept>

// 컨텍스트 없는 단일 플레이어 게임을 위한 기본 알고리즘 클래스
class SinglePlayerWithoutContextBaseAlgorithm : public BaseAlgorithm {
public:
    SinglePlayerWithoutContextBaseAlgorithm(const std::string& name) : BaseAlgorithm(name) {}
    
    // 대부분의 컨텍스트 없는 단일 플레이어 게임에서는 액션이 없으므로 0을 반환
    int selectAction(const GameState& state) override {
        return 0;
    }
    
    // 기본 runAndEvaluate 구현 (AutoMaze에 특화됨)
    std::unique_ptr<GameState> runAndEvaluate(const GameState& state, int action) override {
        auto game_state = dynamic_cast<const AutoMazeState*>(&state);
        if (!game_state) {
            throw std::invalid_argument("SinglePlayerWithoutContextBaseAlgorithm: Invalid state type");
        }
        
        return std::make_unique<AutoMazeState>(*game_state);
        // 실제 상태 변환은 각 알고리즘에서 구현해야 함
    }
};

#endif // SINGLE_PLAYER_WITHOUT_CONTEXT_BASE_H 