#ifndef WALLMAZE_STATE_H
#define WALLMAZE_STATE_H

#include "../../common/game_state.h"
#include "../../common/game_util.h"
#include "../../common/coord.h"
#include "../../algorithms/pathfinding/pathfinding.h"
#include <vector>
#include <string>
#include <memory>
#include <queue>
#include <cstdint>

using ScoreType = GameConstants::ScoreType;

class WallMazeState : public GameState {
private:
    int points_[GameConstants::Board::H][GameConstants::Board::W] = {};
    int walls_[GameConstants::Board::H][GameConstants::Board::W] = {};
    int turn_ = 0;

    // 사용하는 경로 찾기 알고리즘 타입
    PathfindingConstants::Algorithm pathAlgorithmType_ = PathfindingConstants::Algorithm::BFS;

    void initHash();

    int calculateDistance(const Coord& start, const Coord& goal) const;
    
    ScoreType evaluatePotentialScore() const;

public:
    Coord character_ = Coord();
    int game_score_ = 0;
    int first_action_ = -1;
    ScoreType evaluated_score_ = 0;
    uint64_t hash_ = 0;  // Zobrist 해시 값

    WallMazeState();
    WallMazeState(const int seed);

    bool isDone() const override;
    void progress(const int action) override;
    std::vector<int> legalActions() const override;
    std::string toString() const override;
    GameConstants::ScoreType evaluateScore() override;
    
    std::unique_ptr<GameState> clone() const override {
        return std::make_unique<WallMazeState>(*this);
    }
    bool operator<(const GameState& other) const override {
        const WallMazeState& maze_other = static_cast<const WallMazeState&>(other);
        return this->evaluated_score_ < maze_other.evaluated_score_;
    }
    
    // 추가 메서드
    // 특정 게임에만 필요한 비교 연산자
    bool operator<(const WallMazeState& other) const;

    int getDistanceToNearestPoint() const;

    bool hasWall(int y, int x) const { return walls_[y][x] == 1; }
    bool isWalkable(int y, int x) const { return !hasWall(y, x); }
    void setPathAlgorithm(PathfindingConstants::Algorithm type) { pathAlgorithmType_ = type; }
    PathfindingConstants::Algorithm getPathAlgorithm() const { return pathAlgorithmType_; }
    int getDistanceToPoint(const Coord& target, PathfindingConstants::Algorithm algo = PathfindingConstants::Algorithm::BFS) const;
    int getNextActionTowards(const Coord& target, PathfindingConstants::Algorithm algo = PathfindingConstants::Algorithm::BFS) const;
    Coord findNearestPoint(PathfindingConstants::Algorithm algo = PathfindingConstants::Algorithm::BFS) const;
    Coord findHighestValuePoint(PathfindingConstants::Algorithm algo = PathfindingConstants::Algorithm::BFS) const;

    // 비트셋 버전 - 인라인 구현함
    int getTurn() const { return turn_; }
    int getScore() const { return game_score_; }
    Coord getCharacterPosition() const { return character_; }
    int getPoint(int y, int x) const { return points_[y][x]; }

    static PathfindingResult benchmarkPathfinding(
        const WallMazeState& state,
        const Coord& start,
        const Coord& goal,
        PathfindingConstants::Algorithm algo
    );
};

#endif // WALLMAZE_STATE_H