#include "maze_state.h"
#include "../../common/game_util.h"
#include <iostream>
#include <sstream>
#include <random>

MazeState::MazeState() {}

// h*w 크기의 미로를 생성한다.
MazeState::MazeState(const int seed)
{
    auto mt_for_construct = std::mt19937(seed);
    this->character_.y_ = mt_for_construct() % GameConstants::Board::H;
    this->character_.x_ = mt_for_construct() % GameConstants::Board::W;

    GameUtil::generateRandomPoints<GameConstants::Board::H, GameConstants::Board::W>(this->points_, mt_for_construct, 0, 9);
    this->points_[this->character_.y_][this->character_.x_] = 0;
}

// 게임 종료 판정
bool MazeState::isDone() const
{
    return this->turn_ == GameConstants::Board::END_TURN;
}

// 지정한 action으로 게임을 1턴 진행
void MazeState::progress(const int action)
{
    this->character_.x_ += GameConstants::DX[action];
    this->character_.y_ += GameConstants::DY[action];
    auto& point = this->points_[this->character_.y_][this->character_.x_];
    if (point > 0)
    {
        this->game_score_ += point;
        point = 0;
    }
    this->turn_++;
}

// 현재 상황에서 가능한 행동을 모두 수집.
std::vector<int> MazeState::legalActions() const
{
    std::vector<int> actions;
    for (int action = 0; action < 4; action++)
    {
        int ty = this->character_.y_ + GameConstants::DY[action];
        int tx = this->character_.x_ + GameConstants::DX[action];
        if (GameUtil::isValidCoord<GameConstants::Board::H, GameConstants::Board::W>(Coord(ty, tx)))
        {
            actions.emplace_back(action);
        }
    }
    return actions;
}

// 현재 게임 상황을 문자열로 출력
std::string MazeState::toString() const
{
    std::stringstream ss;
    ss << "turn:\t" << this->turn_ << "\n";
    ss << "score:\t" << this->game_score_ << "\n";
    ss << GameUtil::renderSingleCharMaze<GameConstants::Board::H, GameConstants::Board::W>(this->points_, this->character_);
    return ss.str();
}

void MazeState::evaluateScore()
{
    this->evaluated_score_ = this->game_score_;
}   

bool MazeState::operator<(const MazeState& other) const
{
    return this->evaluated_score_ < other.evaluated_score_;
}