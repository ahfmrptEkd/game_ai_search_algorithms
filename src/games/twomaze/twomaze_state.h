#ifndef TWOMAZE_STATE_H
#define TWOMAZE_STATE_H

#include "../../common/game_state.h"
#include "../../common/coord.h"
#include <vector>
#include <string>

using ScoreType = GameConstants::ScoreType;

enum WinningStatus
{
    WIN,
    LOSE,
    DRAW,
    NONE,
};

struct TwoMazePlayer {
    Coord coord_;
    int game_score_;

    TwoMazePlayer(const int y = 0, const int x = 0) : coord_(y, x), game_score_(0) {}
};

class TwoMazeState : public GameState {
private:
    int points_[GameConstants::Board::H][GameConstants::Board::W] = {};
    int turn_;
    TwoMazePlayer players_[GameConstants::TwoMaze::PLAYER_N];
public:
    int first_action_ = -1;
    ScoreType evaluated_score_ = 0;

    TwoMazeState();
    TwoMazeState(const int seed);

    bool isDone() const override;
    void progress(const int action) override;
    std::vector<int> legalActions() const override;
    std::string toString() const override;
    void evaluateScore() override;
    
    // 추가 메서드
    WinningStatus getWinningStatus() const;
    int getCurrentPlayerScore() const;
    int getOpponentScore() const;
    bool operator<(const TwoMazeState& other) const;
};

#endif // TWOMAZE_STATE_H