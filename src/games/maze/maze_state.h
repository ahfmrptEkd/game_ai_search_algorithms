#ifndef MAZE_STATE_H
#define MAZE_STATE_H

#include <vector>
#include <string>
#include "../../common/coord.h"
#include "../../common/game_state.h"

// 중복 정의를 제거하고 공통 상수 사용
using ScoreType = GameConstants::ScoreType;

class MazeState : public GameState {
private:
    int points_[GameConstants::Maze::H][GameConstants::Maze::W] = {};
    int turn_ = 0;

public:
    Coord character_ = Coord();
    int game_score_ = 0;
    int first_action_ = -1;
    ScoreType evaluated_score_ = 0;
    
    MazeState();
    MazeState(const int seed);

    // GameState 인터페이스 구현
    bool isDone() const override;
    void progress(const int action) override;
    std::vector<int> legalActions() const override;
    std::string toString() const override;
    void evaluateScore() override;
    bool operator<(const MazeState& other) const;
};

#endif // MAZE_STATE_H 