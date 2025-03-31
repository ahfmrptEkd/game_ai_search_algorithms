#ifndef AUTOMAZE_STATE_H
#define AUTOMAZE_STATE_H

#include <string>
#include <vector>
#include <random>
#include <functional>
#include "../../common/coord.h"
#include "../../common/game_state.h"

// 중복 정의를 제거하고 공통 상수 사용
using ScoreType = GameConstants::ScoreType;

class AutoMazeState : public GameState {
private:
    int points_[GameConstants::AutoMaze::H][GameConstants::AutoMaze::W] = {};
    int turn_ = 0;
    Coord characters_[GameConstants::AutoMaze::CHARACTER_N] = {};
    void movePlayer(const int character_id);

public:
    int game_score_ = 0;
    ScoreType evaluated_score_ = 0;

    AutoMazeState(const int seed);

    // GameState 인터페이스 구현
    bool isDone() const override;
    void progress(const int action) override; // 입력 액션은 무시됨
    std::vector<int> legalActions() const override;
    std::string toString() const override;
    void evaluateScore() override;

    // 추가 메서드
    ScoreType getScore(bool is_print = false) const;
    void setCharacter(const int character_id, const int y, const int x);
    bool operator<(const AutoMazeState& other) const;
};

#endif // AUTOMAZE_STATE_H