#ifndef CONNECT_FOUR_STATE_H
#define CONNECT_FOUR_STATE_H

#include "../../common/game_constants.h"
#include "../../common/game_util.h"
#include <vector>
#include <string>
#include <deque>
#include <utility>
#include <sstream>
#include <memory>

using ScoreType = GameConstants::ScoreType;

class ConnectFourState : public GameState {
private:
    static constexpr const int dx[2] = {1, -1};
    static constexpr const int dy_right_up[2] = {1, -1}; // "/" 방향
    static constexpr const int dy_left_up[2] = {-1, 1}; // "\" 방향

    bool is_first_ = true;  // 선공 여부
    int my_board_[GameConstants::ConnectFour::H][GameConstants::ConnectFour::W];
    int enemy_board_[GameConstants::ConnectFour::H][GameConstants::ConnectFour::W];
    WinningStatus winning_status_ = WinningStatus::NONE;

public:
    ConnectFourState();
    ConnectFourState(const int seed);

    ScoreType evaluated_score_ = 0;

    // GameState 인터페이스 구현
    bool isDone() const override;
    void progress(const int action) override;
    std::vector<int> legalActions() const override;
    std::string toString() const override;
    ScoreType evaluateScore() override;
    
    std::unique_ptr<GameState> clone() const override {
        return std::make_unique<ConnectFourState>(*this);
    }

    bool operator<(const GameState& other) const override {
        const ConnectFourState& connectfour_other = static_cast<const ConnectFourState&>(other);
        return this->evaluated_score_ < connectfour_other.evaluated_score_;
    }

    WinningStatus getWinningStatus() const;
    double getFirstPlayerScoreForWinRate() const;
    bool isFirst() const { return is_first_; }

    // 비트보드 변환 유틸리티
    const int* getMyBoard() const { return &my_board_[0][0]; }
    const int* getEnemyBoard() const { return &enemy_board_[0][0]; }
};

#endif // CONNECT_FOUR_STATE_H