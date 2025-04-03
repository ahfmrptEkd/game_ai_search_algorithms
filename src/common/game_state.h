#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <string>
#include <memory>
#include "game_constants.h"

// 게임 상태 인터페이스
// 모든 게임 구현체가 공통으로 구현해야 하는 인터페이스
class GameState {
public:
    virtual ~GameState() = default;

    // 게임이 종료되었는지 확인
    virtual bool isDone() const = 0;

    // 지정한 행동으로 게임을 진행
    virtual void progress(const int action) = 0;

    // 현재 상황에서 가능한 행동을 모두 수집
    virtual std::vector<int> legalActions() const = 0;

    // 현재 게임 상황을 문자열로 출력
    virtual std::string toString() const = 0;

    // 현재 게임 상황을 평가 - 점수 반환 추가
    virtual GameConstants::ScoreType evaluateScore() = 0;

    // 상태 복제 함수 추가
    virtual std::unique_ptr<GameState> clone() const = 0;
    
    // 상태 비교 함수 추가
    virtual bool operator<(const GameState& other) const = 0;
    
    // 템플릿 메소드 추가 - 공통 동작 구현
    template<typename StateType>
    static std::vector<StateType> getNextStates(const StateType& state) {
        std::vector<StateType> next_states;
        for (const auto& action : state.legalActions()) {
            StateType next_state = state;
            next_state.progress(action);
            next_states.push_back(next_state);
        }
        return next_states;
    }
};

#endif // GAME_STATE_H 