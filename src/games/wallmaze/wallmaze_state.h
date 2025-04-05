#ifndef WALLMAZE_STATE_H
#define WALLMAZE_STATE_H

#include "../../common/game_state.h"
#include "../../common/game_util.h"
#include "../../common/coord.h"
#include <vector>
#include <string>
#include <memory>

using ScoreType = GameConstants::ScoreType;

class WallMazeState : public GameState {
private:
    int points_[GameConstants::Board::H][GameConstants::Board::W] = {};
    int walls_[GameConstants::Board::H][GameConstants::Board::W] = {};
    int turn_ = 0;

    // BFS로 시작점에서 목표까지 최단 거리 계산
    int bfsDistance(const Coord& start, const Coord& goal) const;
    ScoreType evaluatePotentialScore() const;

public:
    Coord character_ = Coord();
    int game_score_ = 0;
    int first_action_ = -1;
    ScoreType evaluated_score_ = 0;

    WallMazeState();
    WallMazeState(const int seed);

    bool isDone() const override;
    void progress(const int action) override;
    std::vector<int> legalActions() const override;
    std::string toString() const override;
    GameConstants::ScoreType evaluateScore() override;
    
    std::unique_ptr<GameState> clone() const override {
        return std::make_unique<WallMazeState>(*this);
    }
    bool operator<(const GameState& other) const override {
        const WallMazeState& maze_other = static_cast<const WallMazeState&>(other);
        return this->evaluated_score_ < maze_other.evaluated_score_;
    }
    
    // 특정 게임에만 필요한 비교 연산자
    bool operator<(const WallMazeState& other) const;

    bool hasWall(int y, int x) const { return walls_[y][x] == 1; }
};

#endif // WALLMAZE_STATE_H