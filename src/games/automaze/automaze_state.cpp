#include "automaze_state.h"
#include "../../common/game_util.h"
#include <iostream>
#include <sstream>
#include <random>

// h*w 크기의 미로를 생성한다.
AutoMazeState::AutoMazeState(const int seed)
    : turn_(0), game_score_(0), evaluated_score_(0)
{
    auto mt_for_construct = std::mt19937(seed);
    GameUtil::generateRandomPoints<GameConstants::Board::H, GameConstants::Board::W>(this->points_, mt_for_construct, 0, 9);
}

// 지정 위치에 지정 캐릭터를 배치한다.
void AutoMazeState::setCharacter(const int character_id, const int y, const int x)
{
    this->characters_[character_id].y_ = y;
    this->characters_[character_id].x_ = x;
}

// 지정 캐릭터를 이동시킨다.
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
    for (auto &character : this->characters_)
    {
        auto &point = this->points_[character.y_][character.x_];
        this->game_score_ += point;
        point = 0;
    }
    ++this->turn_;
}

// 게임 종료 판정
bool AutoMazeState::isDone() const
{
    return this->turn_ == GameConstants::Board::END_TURN;
}

// 현재 상황에서 가능한 행동을 모두 수집
std::vector<int> AutoMazeState::legalActions() const
{
    // 단순화를 위해 모든 캐릭터 위치 조합을 가능한 액션으로 반환
    // 실제 구현에서는 중복 위치 제거 등 추가 로직 필요
    std::vector<int> actions;
    int max_actions = pow(GameConstants::Board::H * GameConstants::Board::W, GameConstants::AutoMaze::CHARACTER_N);
    
    // 액션 수가 너무 많을 수 있어 실제 구현에서는 제한이 필요할 수 있음
    for (int i = 0; i < max_actions; i++) {
        actions.push_back(i);
    }
    return actions;
}

// 현재 게임 상황을 문자열로 출력
std::string AutoMazeState::toString() const
{
    std::stringstream ss;
    ss << "turn:\t" << this->turn_ << "\n";
    ss << "score:\t" << this->game_score_ << "\n";
    ss << GameUtil::renderMultiCharMaze<GameConstants::Board::H, GameConstants::Board::W, GameConstants::AutoMaze::CHARACTER_N>(this->points_, this->characters_);
    return ss.str();
}

// 현재 게임 상태 평가
void AutoMazeState::evaluateScore()
{
    this->evaluated_score_ = this->getScore();
}

// 게임 점수 계산
ScoreType AutoMazeState::getScore(bool is_print) const
{
    auto tmp_state = *this;
    // 이미 현재 상태에서 점수가 계산되었다면 그대로 반환
    if (tmp_state.isDone()) {
        return tmp_state.game_score_;
    }
    
    // 종료할 때까지 캐릭터 이동을 반복한다.
    while (!tmp_state.isDone())
    {
        tmp_state.progress(0); // 인수 0은 무시됨
        if (is_print)
            std::cout << tmp_state.toString() << std::endl;
    }
    return tmp_state.game_score_;
}

// 연산자 오버로딩
bool AutoMazeState::operator<(const AutoMazeState& other) const
{
    return this->evaluated_score_ < other.evaluated_score_;
}