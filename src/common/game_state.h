#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <string>
#include <cstdint>

// 모든 게임에서 공통으로 사용하는 상수
namespace GameConstants {
    using ScoreType = int64_t;
    constexpr const ScoreType INF = 1000000000LL;
    
    constexpr const int DX[4] = {1, -1, 0, 0};
    constexpr const int DY[4] = {0, 0, 1, -1};
    
    // 모든 게임에 공통으로 사용되는 보드 관련 상수
    namespace Board {
        constexpr const int H = 5;
        constexpr const int W = 5;
        constexpr const int END_TURN = 5;
    }
    
    namespace AutoMaze {
        // AutoMaze에만 있는 특수 상수
        constexpr const int CHARACTER_N = 3;
    }
    
    // 알고리즘 파라미터 상수 추가
    namespace AlgorithmParams {
        constexpr const int SEARCH_WIDTH = 2;
        constexpr const int SEARCH_DEPTH = 4;
        constexpr const int SEARCH_NUMBER = 2;
        constexpr const int64_t TIME_THRESHOLD = 1;
    }
}

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

    // 현재 게임 상황을 평가
    virtual void evaluateScore() = 0;
};

#endif // GAME_STATE_H 