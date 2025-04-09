#include "simmaze_state.h"
#include "../../common/game_util.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

SimMazeState::SimMazeState() {}

SimMazeState::SimMazeState(const int seed) 
    : turn_(0)
{
    auto mt_for_construct = GameUtil::mt_for_action;

    // 초기 플레이어 위치 설정
    players_[0] = SimMazePlayer(GameConstants::Board::H / 2, (GameConstants::Board::W / 2) - 1);
    players_[1] = SimMazePlayer(GameConstants::Board::H / 2, (GameConstants::Board::W / 2) + 1);

    // 점수맵 - 대칭 설정
    for (int y = 0; y < GameConstants::Board::H; y++) {
        for (int x = 0; x < GameConstants::Board::W; x++) {
            bool is_player = false;
            for (const auto& player : players_) {
                if (player.coord_.y_ == y && player.coord_.x_ == x) {
                    is_player = true;
                    break;
                }

                if (player.coord_.y_ == y && player.coord_.x_ == (GameConstants::Board::W - 1 - x)) {
                    is_player = true;
                    break;
                }
            }

            if (!is_player) {
                int point = mt_for_construct() % 10;
                this->points_[y][x] = point;
                this->points_[y][GameConstants::Board::W - 1 - x] = point;
            }
            else {
                this->points_[y][x] = 0;
                this->points_[y][GameConstants::Board::W - 1 - x] = 0;
            }
        }
    }
}

bool SimMazeState::isDone() const
{
    return this->turn_ >= GameConstants::Board::END_TURN;
}

void SimMazeState::progress(const int encoded_action)
{
    // 인코딩된 액션을 디코딩하여 두 플레이어의 액션을 분리
    std::pair<int, int> actions = SimMazeState::decodeActions(encoded_action);
    int action0 = actions.first;
    int action1 = actions.second;

    // 디코딩된 액션으로 게임 진행
    advance(action0, action1);
}

void SimMazeState::advance(const int action0, const int action1)
{
    // 마지막 액션 저장
    last_actions_[0] = action0;
    last_actions_[1] = action1;

    for (int player_id = 0; player_id < GameConstants::TwoMaze::PLAYER_N; player_id++) {
        auto& player = this->players_[player_id];
        const int action = (player_id == 0) ? action0 : action1;

        player.coord_.y_ += GameConstants::DY[action];
        player.coord_.x_ += GameConstants::DX[action];
    }

    // 점수 획득 처리 - (플레이어가 동시에 위치에 이동한 경우 누구도 점수 없지 않음)
    bool collison = (players_[0].coord_.y_ == players_[1].coord_.y_ && players_[0].coord_.x_ == players_[1].coord_.x_);

    if (!collison) {
        for (int player_id = 0; player_id < GameConstants::TwoMaze::PLAYER_N; player_id++) {
            auto& player = this->players_[player_id];
            auto& point = this->points_[player.coord_.y_][player.coord_.x_];
            if (point > 0) {
                player.game_score_ += point;
                point = 0;
            }
        }
    }
    else {
        auto& point = this->points_[players_[0].coord_.y_][players_[0].coord_.x_];
        point = 0;
    }

    this->turn_++;
}

std::vector<int> SimMazeState::legalActions() const
{
    // 두 플레이어의 모든 가능한 액션 조합 반환
    std::vector<int> actions;
    auto actions0 = legalActions(0);
    auto actions1 = legalActions(1);
    
    for (const auto action0 : actions0) {
        for (const auto action1 : actions1) {
            actions.push_back(encodeActions(action0, action1));
        }
    }
    
    return actions;
}

std::vector<int> SimMazeState::legalActions(const int player_id) const
{
    std::vector<int> actions;
    const auto& player = this->players_[player_id];
    
    for (int action = 0; action < 4; action++) {
        int ty = player.coord_.y_ + GameConstants::DY[action];
        int tx = player.coord_.x_ + GameConstants::DX[action];
        
        if (GameUtil::isValidCoord<GameConstants::Board::H, GameConstants::Board::W>(Coord(ty, tx))) {
            actions.emplace_back(action);
        }
    }
    return actions;
}

std::string SimMazeState::toString() const
{
    std::stringstream ss;
    ss << "turn:\t" << this->turn_ << "\n";

    for (int player_id = 0; player_id < GameConstants::TwoMaze::PLAYER_N; player_id++) {
        const auto& player = this->players_[player_id];
        ss << "score(" << player_id << "):\t" << player.game_score_;
        
        if (last_actions_[player_id] >= 0) {
            ss << "\tlast action: ";
            switch (last_actions_[player_id]) {
                case 0: ss << "RIGHT"; break;
                case 1: ss << "LEFT"; break;
                case 2: ss << "DOWN"; break;
                case 3: ss << "UP"; break;
                default: ss << "UNKNOWN"; break;
            }
        }
        ss << "\n";
    }

    for (int h = 0; h < GameConstants::Board::H; h++) {
        for (int w = 0; w < GameConstants::Board::W; w++) {
            bool is_player = false;
            for (int player_id = 0; player_id < GameConstants::TwoMaze::PLAYER_N; player_id++) {
                const auto& player = this->players_[player_id];
                if (player.coord_.y_ == h && player.coord_.x_ == w) {
                    ss << (player_id == 0 ? "A" : "B");
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

GameConstants::ScoreType SimMazeState::evaluateScore()
{
    this->evaluated_score_ = static_cast<GameConstants::ScoreType>(
        this->players_[0].game_score_ - this->players_[1].game_score_);
    return this->evaluated_score_;
}

WinningStatus SimMazeState::getWinningStatus() const
{
    if (this->isDone()) {
        const int player0_score = this->players_[0].game_score_;
        const int player1_score = this->players_[1].game_score_;
        
        if (player0_score > player1_score) {
            return WinningStatus::WIN;
        } else if (player0_score < player1_score) {
            return WinningStatus::LOSE;
        } else {
            return WinningStatus::DRAW;
        }
    } 
    return WinningStatus::NONE;
}

bool SimMazeState::operator<(const SimMazeState& other) const
{
    return this->evaluated_score_ < other.evaluated_score_;
}

double SimMazeState::getScoreRate() const
{
    const int player0_score = this->players_[0].game_score_;
    const int player1_score = this->players_[1].game_score_;
    
    int total_score = player0_score + player1_score;
    if (total_score == 0)
        return 0.5;
        
    return static_cast<double>(player0_score) / static_cast<double>(total_score);
}

int SimMazeState::encodeActions(int action0, int action1)
{
    return ENCODE_ACTION_PAIR(action0, action1);
}

std::pair<int, int> SimMazeState::decodeActions(int encoded_action)
{
    return {DECODE_ACTION0(encoded_action), DECODE_ACTION1(encoded_action)};
}