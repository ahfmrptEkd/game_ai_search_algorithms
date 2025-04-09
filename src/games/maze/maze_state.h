#ifndef MAZE_STATE_H
#define MAZE_STATE_H

#include <vector>
#include <string>
#include <memory>
#include "../../common/coord.h"
#include "../../common/game_state.h"

using ScoreType = GameConstants::ScoreType;

class MazeState : public GameState {
private:
    int points_[GameConstants::Board::H][GameConstants::Board::W] = {};
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
    
    // 평가 점수 반환 메소드로 변경
    GameConstants::ScoreType evaluateScore() override;
    
    // 상태 복제 메소드 추가
    std::unique_ptr<GameState> clone() const override {
        return std::make_unique<MazeState>(*this);
    }
    
    // 상태 비교 연산자 구현
    bool operator<(const GameState& other) const override {
        const MazeState& maze_other = static_cast<const MazeState&>(other);
        return this->evaluated_score_ < maze_other.evaluated_score_;
    }
    
    bool operator<(const MazeState& other) const;
};

#endif // MAZE_STATE_H 