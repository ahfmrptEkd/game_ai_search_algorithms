#include "automaze_state.h"
#include "../../common/game_util.h"
#include <iostream>
#include <sstream>
#include <random>

AutoMazeState::AutoMazeState(const int seed)
    : turn_(0), game_score_(0), evaluated_score_(0)
{
    auto mt_for_construct = std::mt19937(seed);
    GameUtil::generateRandomPoints<GameConstants::Board::H, GameConstants::Board::W>(this->points_, mt_for_construct, 0, 9);
}

void AutoMazeState::setCharacter(const int character_id, const int y, const int x)
{
    this->characters_[character_id].y_ = y;
    this->characters_[character_id].x_ = x;
}

void AutoMazeState::movePlayer(const int character_id)
{
    Coord &character = this->characters_[character_id];
    int best_point = -GameConstants::INF;
    int best_action_index = 0;
    for (int action = 0; action < 4; action++)
    {
        int ty = character.y_ + GameConstants::DY[action];
        int tx = character.x_ + GameConstants::DX[action];
        if (ty >= 0 && ty < GameConstants::Board::H && tx >= 0 && tx < GameConstants::Board::W)
        {
            auto point = this->points_[ty][tx];
            if (point > best_point)
            {
                best_point = point;
                best_action_index = action;
            }
        }
    }

    character.y_ += GameConstants::DY[best_action_index];
    character.x_ += GameConstants::DX[best_action_index];
}

// 게임을 1턴 진행한다. (parameter는 무시)
void AutoMazeState::progress(const int action)
{
    for (int character_id = 0; character_id < GameConstants::AutoMaze::CHARACTER_N; character_id++)
    {
        movePlayer(character_id);
    }
    for (auto& character : this->characters_)
    {
        auto& point = this->points_[character.y_][character.x_];
        this->game_score_ += point;
        point = 0;
    }
    ++this->turn_;
}

bool AutoMazeState::isDone() const
{
    return this->turn_ == GameConstants::Board::END_TURN;
}

std::vector<int> AutoMazeState::legalActions() const
{
    // 단순화를 위해 모든 캐릭터 위치 조합을 가능한 액션으로 반환
        // 중복 위치 제거 등 추가 로직 필요
    std::vector<int> actions;
    int max_actions = pow(GameConstants::Board::H * GameConstants::Board::W, GameConstants::AutoMaze::CHARACTER_N);
    
    for (int i = 0; i < max_actions; i++) {
        actions.push_back(i);
    }
    return actions;
}

std::string AutoMazeState::toString() const
{
    std::stringstream ss;
    ss << "turn:\t" << this->turn_ << "\n";
    ss << "score:\t" << this->game_score_ << "\n";
    ss << GameUtil::renderMultiCharMaze<GameConstants::Board::H, GameConstants::Board::W, GameConstants::AutoMaze::CHARACTER_N>(this->points_, this->characters_);
    return ss.str();
}

GameConstants::ScoreType AutoMazeState::evaluateScore()
{
    this->evaluated_score_ = this->getScore();
    return this->evaluated_score_;
}

ScoreType AutoMazeState::getScore(bool is_print) const
{
    auto tmp_state = *this;
    if (tmp_state.isDone()) {
        return tmp_state.game_score_;
    }
    
    while (!tmp_state.isDone())
    {
        tmp_state.progress(0); // 인수 0은 무시됨
        if (is_print)
            std::cout << tmp_state.toString() << std::endl;
    }
    return tmp_state.game_score_;
}

bool AutoMazeState::operator<(const AutoMazeState& other) const
{
    return this->evaluated_score_ < other.evaluated_score_;
}