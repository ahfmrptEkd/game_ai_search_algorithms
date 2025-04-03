#ifndef TWOMAZE_STATE_H
#define TWOMAZE_STATE_H

#include "../../common/game_state.h"
#include "../../common/game_util.h"
#include "../../common/coord.h"
#include <vector>
#include <string>
#include <memory>

using ScoreType = GameConstants::ScoreType;

struct TwoMazePlayer {
    Coord coord_;
    int game_score_;

    TwoMazePlayer(const int y = 0, const int x = 0) : coord_(y, x), game_score_(0) {}
};

class TwoMazeState : public GameState {
private:
    int points_[GameConstants::Board::H][GameConstants::Board::W] = {};
    int turn_ = 0;
    TwoMazePlayer players_[GameConstants::TwoMaze::PLAYER_N];
public:
    int first_action_ = -1;
    ScoreType evaluated_score_ = 0;

    TwoMazeState();
    TwoMazeState(const int seed);
    int getCurrentTurn() const;

    bool isDone() const override;
    void progress(const int action) override;
    std::vector<int> legalActions() const override;
    std::string toString() const override;
    GameConstants::ScoreType evaluateScore() override;
    
    // 복제 및 비교 메소드 추가
    std::unique_ptr<GameState> clone() const override {
        return std::make_unique<TwoMazeState>(*this);
    }
    
    bool operator<(const GameState& other) const override {
        const TwoMazeState& maze_other = static_cast<const TwoMazeState&>(other);
        return this->evaluated_score_ < maze_other.evaluated_score_;
    }
    
    // 추가 메서드
    WinningStatus getWinningStatus() const;
    int getCurrentPlayerScore() const;
    int getOpponentScore() const;
    bool operator<(const TwoMazeState& other) const;
    double getScoreRate() const;
};

#endif // TWOMAZE_STATE_H