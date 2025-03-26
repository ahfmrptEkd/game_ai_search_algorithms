#include "maze_state.h"
#include <iostream>
#include <sstream>
#include <random>

MazeState::MazeState() {}

// h*w 크기의 미로를 생성한다.
MazeState::MazeState(const int seed)
{
    auto mt_for_construct = std::mt19937(seed);
    this->character_.y_ = mt_for_construct() % H;
    this->character_.x_ = mt_for_construct() % W;

    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
        {
            if (y == character_.y_ && x == character_.x_)
            {
                continue;
            }
            this->points_[y][x] = mt_for_construct() % 10;
        }
}

// 게임 종료 판정
bool MazeState::isDone() const
{
    return this->turn_ == END_TURN;
}

// 지정한 action으로 게임을 1턴 진행
void MazeState::progress(const int action)
{
    this->character_.x_ += DX[action];
    this->character_.y_ += DY[action];
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
        int ty = this->character_.y_ + DY[action];
        int tx = this->character_.x_ + DX[action];
        if (ty >= 0 && ty < H && tx >= 0 && tx < W)
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

    for (int h = 0; h < H; h++)
    {
        for (int w = 0; w < W; w++)
        {
            if (this->character_.y_ == h && this->character_.x_ == w)
            {
                ss << '@';
            }
            else if (this->points_[h][w] > 0)
            {
                ss << points_[h][w];
            }
            else
            {
                ss << '.';
            }
        }
        ss << '\n';
    }
    
    return ss.str();
} 