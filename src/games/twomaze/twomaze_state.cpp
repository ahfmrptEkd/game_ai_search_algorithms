#include "twomaze_state.h"
#include "../../common/game_util.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <random>

TwoMazeState::TwoMazeState(){}

TwoMazeState::TwoMazeState(const int seed)
    : turn_(0)
{
    auto mt_for_construct = GameUtil::mt_for_action;

    //초기 플레이어 위치 설정
    players_[0] = TwoMazePlayer(GameConstants::Board::H / 2, (GameConstants::Board::W / 2) - 1);
    players_[1] = TwoMazePlayer(GameConstants::Board::H / 2, (GameConstants::Board::W / 2) + 1);

    // 점수 맵 초기화
    for (int y = 0; y < GameConstants::Board::H; y++)
        for (int x = 0; x < GameConstants::Board::W; x++)
        {
            bool is_player = false;
            for (const auto& player : players_)
            {
                if (player.coord_.y_ == y && player.coord_.x_ == x)
                {
                    is_player = true;
                    break;
                }
            }
            if (!is_player)
            {
                GameUtil::generateRandomPoints<GameConstants::Board::H, GameConstants::Board::W>(this->points_, mt_for_construct, 0, 9);
            }
            else
            {
                this->points_[y][x] = 0;
            }
        }
}

bool TwoMazeState::isDone() const
{
    return this->turn_ >= GameConstants::Board::END_TURN;
}

void TwoMazeState::progress(const int action)
{
    auto& player = this->players_[0];
    player.coord_.y_ += GameConstants::DY[action];
    player.coord_.x_ += GameConstants::DX[action];

    auto& point = this->points_[player.coord_.y_][player.coord_.x_];
    if (point > 0)
    {
        player.game_score_ += point;
        point = 0;
    }

    this->turn_++;

    std::swap(players_[0], players_[1]);
}

std::vector<int> TwoMazeState::legalActions() const
{
    std::vector<int> actions;
    const auto& player = this->players_[0];
    for (int action = 0; action < 4; action++)
    {
        int ty = player.coord_.y_ + GameConstants::DY[action];
        int tx = player.coord_.x_ + GameConstants::DX[action];
        
        if (GameUtil::isValidCoord<GameConstants::Board::H, GameConstants::Board::W>(Coord(ty, tx)))
        {
            actions.emplace_back(action);
        }
    }
    return actions;
}

std::string TwoMazeState::toString() const
{
    std::stringstream ss;
    ss << "turn:\t" << this->turn_ << "\n";

    for (int player_id = 0; player_id < GameConstants::TwoMaze::PLAYER_N; player_id++)
    {
        int actual_player_id = player_id;
        if (this->turn_ % 2 == 1)
        {
            actual_player_id = (player_id + 1) % 2; // 홀수 턴은 초기 배치 시점에서 보면 player_id가 반대
        }
        const auto& player = this->players_[actual_player_id];
        ss << "score:\t" << player_id << ":\t" << player.game_score_ << "\ty: " << player.coord_.y_ << "\tx: " << player.coord_.x_ << "\n";
    }

    // 게임 보드
    for (int h = 0; h < GameConstants::Board::H; h++) {
        for (int w = 0; w < GameConstants::Board::W; w++) {
            bool is_player = false;
            for (int player_id = 0; player_id < GameConstants::TwoMaze::PLAYER_N; player_id++) {
                int actual_player_id = player_id;
                if (this->turn_ % 2 == 1) {
                    actual_player_id = (player_id + 1) % 2;
                }
                const auto& player = this->players_[player_id];
                if (player.coord_.y_ == h && player.coord_.x_ == w) {
                    ss << (actual_player_id == 0 ? "A" : "B");
                    is_player = true;
                    break;
                }
            }
            if (!is_player) {
                if (this->points_[h][w] > 0) {
                    ss << this->points_[h][w];
                } else {
                    ss << ".";
                }
            }
        }
        ss << "\n";
    }
    return ss.str();
}

GameConstants::ScoreType TwoMazeState::evaluateScore() {
    // 기본적인 평가: 현재 플레이어 점수 - 상대 플레이어 점수
    this->evaluated_score_ = static_cast<GameConstants::ScoreType>(this->getCurrentPlayerScore() - this->getOpponentScore());
    return this->evaluated_score_;
}

WinningStatus TwoMazeState::getWinningStatus() const {
    if (isDone()) {
        int player0_score, player1_score;
        
        if (this->turn_ % 2 == 0) {
            // 짝수 턴: 원래 순서대로
            player0_score = players_[0].game_score_;
            player1_score = players_[1].game_score_;
        } else {
            // 홀수 턴: 교체된 순서 (players_[1]이 원래 player0, players_[0]이 원래 player1)
            player0_score = players_[1].game_score_;
            player1_score = players_[0].game_score_;
        }
        
        if (player0_score > player1_score) {
            return WinningStatus::WIN; 
        } else if (player0_score < player1_score) {
            return WinningStatus::LOSE; 
        } else {
            return WinningStatus::DRAW;
        }
    } else {
        return WinningStatus::NONE;
    }
}

int TwoMazeState::getCurrentPlayerScore() const {
    return players_[0].game_score_;
}

int TwoMazeState::getOpponentScore() const {
    return players_[1].game_score_;
}

bool TwoMazeState::operator<(const TwoMazeState& other) const {
    return this->evaluated_score_ < other.evaluated_score_;
}

double TwoMazeState::getScoreRate() const {
    int player0_score, player1_score;
    
    if (this->turn_ % 2 == 0) {
        // 짝수 턴: 원래 순서
        player0_score = players_[0].game_score_;
        player1_score = players_[1].game_score_;
    } else {
        // 홀수 턴: 교체된 순서
        player0_score = players_[1].game_score_;
        player1_score = players_[0].game_score_;
    }
    
    int total_score = player0_score + player1_score;
    if (total_score == 0)
        return 0.5; // 0으로 나누기 방지, 무승부 상태로 처리
        
    return static_cast<double>(player0_score) / static_cast<double>(total_score);
}

int TwoMazeState::getCurrentTurn() const {
    return this->turn_;
}