#ifndef SIMMAZE_STATE_H
#define SIMMAZE_STATE_H

#include "../../common/game_state.h"
#include "../../common/game_util.h"
#include "../../common/coord.h"
#include <vector>
#include <string>
#include <memory>
#include <array>

using ScoreType = GameConstants::ScoreType;

// 디버깅용
#define ENCODE_ACTION_PAIR(a0, a1) ((a0) * 4 + (a1))
#define DECODE_ACTION0(action) ((action) / 4)
#define DECODE_ACTION1(action) ((action) % 4)

struct SimMazePlayer {
    Coord coord_;
    int game_score_;

    SimMazePlayer(const int y = 0, const int x = 0) : coord_(y, x), game_score_(0) {}
};

class SimMazeState : public GameState {
private:
    int points_[GameConstants::Board::H][GameConstants::Board::W] = {};
    int turn_ = 0;
    SimMazePlayer players_[GameConstants::TwoMaze::PLAYER_N];

    // 마지막 수행된 액션 저장 - 디버깅
    int last_actions_[GameConstants::TwoMaze::PLAYER_N] = {-1, -1};

public:
    ScoreType evaluated_score_ = 0;

    SimMazeState();
    SimMazeState(const int seed);

    bool isDone() const override;
    
    // 인코딩된 단일 액션으로 게임을 진행 (GameState 인터페이스 호환용)
    void progress(const int encoded_action) override;
    
    std::vector<int> legalActions() const override;
    std::string toString() const override;
    GameConstants::ScoreType evaluateScore() override;
    
    std::unique_ptr<GameState> clone() const override {
        return std::make_unique<SimMazeState>(*this);
    }
    
    bool operator<(const GameState& other) const override {
        const SimMazeState& maze_other = static_cast<const SimMazeState&>(other);
        return this->evaluated_score_ < maze_other.evaluated_score_;
    }
    
    // 추가 메서드
    // 두 플레이어의 액션을 각각 받아 게임을 진행
    void advance(const int action0, const int action1);
    
    // 특정 플레이어의 가능한 액션 목록 반환
    std::vector<int> legalActions(const int player_id) const;
    
    WinningStatus getWinningStatus() const;
    int getPlayerScore(int player_id) const { return players_[player_id].game_score_; }
    bool operator<(const SimMazeState& other) const;    
    double getScoreRate() const;
    
    // 액션 인코딩/디코딩 유틸리티
    static int encodeActions(int action0, int action1);
    static std::pair<int, int> decodeActions(int encoded_action);

    // MCTS 알고리즘 support methods
    int getTurn() const { return turn_; }
    int getPoint(int y, int x) const { return points_[y][x]; }
    int getPlayerX(int player_id) const { return players_[player_id].coord_.x_; }
    int getPlayerY(int player_id) const { return players_[player_id].coord_.y_; }
    int getLastAction(int player_id) const { return last_actions_[player_id]; }
};

#endif // SIMMAZE_STATE_H