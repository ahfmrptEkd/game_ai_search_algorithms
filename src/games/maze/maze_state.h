#ifndef MAZE_STATE_H
#define MAZE_STATE_H

#include <vector>
#include <string>
#include "../../common/coord.h"
#include "../../common/game_state.h"

using ScoreType = int64_t;  // 게임 평가 점수 자료형 결정
constexpr const ScoreType INF = 1000000000LL;

constexpr const int H = 3;
constexpr const int W = 4;
constexpr int END_TURN = 4;

// 방향 상수 정의
constexpr const int DX[4] = {1, -1, 0, 0};
constexpr const int DY[4] = {0, 0, 1, -1};

class MazeState : public GameState {
private:
    int points_[H][W] = {};
    int turn_ = 0;

public:
    Coord character_ = Coord();
    int game_score_ = 0;
    ScoreType evaluated_score_ = 0;
    
    MazeState();
    MazeState(const int seed);

    // GameState 인터페이스 구현
    bool isDone() const override;
    void progress(const int action) override;
    std::vector<int> legalActions() const override;
    std::string toString() const override;
    void evaluateScore() override;
};

using State = MazeState;

#endif // MAZE_STATE_H 