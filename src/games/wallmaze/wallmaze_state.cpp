#include "wallmaze_state.h"
#include "zobrist_hash.h"
#include "../../common/game_util.h"
#include "../../algorithms/pathfinding/pathfinding.h"
#include <iostream>
#include <sstream>
#include <random>
#include <deque>

WallMazeState::WallMazeState() {}

void WallMazeState::initHash()
{
    hash_ = 0;
    
    // 캐릭터 위치 해시
    hash_ ^= zobrist_hash::character[character_.y_][character_.x_];
    
    // 점수 위치 해시
    for (int y = 0; y < GameConstants::Board::H; y++) {
        for (int x = 0; x < GameConstants::Board::W; x++) {
            auto point = points_[y][x];
            if (point > 0) {
                hash_ ^= zobrist_hash::points[y][x][point];
            }
        }
    }
}

WallMazeState::WallMazeState(const int seed) 
{
    auto mt_for_construct = GameUtil::mt_for_action;
    this->character_.y_ = mt_for_construct() % GameConstants::Board::H;
    this->character_.x_ = mt_for_construct() % GameConstants::Board::W;

    // 기둥 쓰러뜨리기 algorithm 으로 미로의 벽 생성
    for (int y = 1; y < GameConstants::Board::H - 1; y += 2) {
        for (int x = 1; x < GameConstants::Board::W - 1; x += 2) {
            int ty = y;
            int tx = x;
            if (ty == character_.y_ && tx == character_.x_) {
                continue;
            }

            this->walls_[ty][tx] = 1;
            int direction_size = 3; // (오른쪽, 왼쪽, 아래쪽) 방향의 근접한 칸을 벽후보로 지정.

            if (y == 1) {
                direction_size = 4; // 첫 행은 위쪽 방향의 근접한 칸도 벽 후보로 추가
            }

            int direction = mt_for_construct() % direction_size;

            ty += GameConstants::DY[direction];
            tx += GameConstants::DX[direction];

            // 이때 (ty, tx)는 1칸 씩 건너뛴 위치에서 무작위로 이동한 인접한 위치
            if (ty == character_.y_ && tx == character_.x_) {
                continue;
            }

            this->walls_[ty][tx] = 1;
        }
    }

    for (int y = 0; y < GameConstants::Board::H; y++) {
        for (int x = 0; x < GameConstants::Board::W; x++) {
            if (y == character_.y_ && x == character_.x_)
            {
                continue;
            }

            this->points_[y][x] = mt_for_construct() % 10;
        }
    }
    initHash();
}

bool WallMazeState::isDone() const
{
    return this->turn_ == GameConstants::Board::END_TURN;
}

void WallMazeState::progress(const int action)
{
    // 현재 캐릭터 위치 정보를 해시에서 제거
    hash_ ^= zobrist_hash::character[character_.y_][character_.x_];
    
    this->character_.x_ += GameConstants::DX[action];
    this->character_.y_ += GameConstants::DY[action];
    
    // 새 캐릭터 위치 정보를 해시에 추가
    hash_ ^= zobrist_hash::character[character_.y_][character_.x_];
    
    auto &point = this->points_[this->character_.y_][this->character_.x_];
    if (point > 0)
    {
        // 점수가 없어진 것을 해시에 반영
        hash_ ^= zobrist_hash::points[character_.y_][character_.x_][point];
        
        this->game_score_ += point;
        point = 0;
    }
    this->turn_++;
}

std::vector<int> WallMazeState::legalActions() const
{
    std::vector<int> actions;
    for (int action = 0; action < 4; action++)
    {
        int ty = this->character_.y_ + GameConstants::DY[action];
        int tx = this->character_.x_ + GameConstants::DX[action];
        if (GameUtil::isValidCoord<GameConstants::Board::H, GameConstants::Board::W>(Coord(ty, tx)) && 
            this->walls_[ty][tx] == 0)
        {
            actions.emplace_back(action);
        }
    }
    return actions;
}

std::string WallMazeState::toString() const
{
    std::stringstream ss;
    ss << "turn:\t" << this->turn_ << "\n";
    ss << "score:\t" << this->game_score_ << "\n";
    for (int h = 0; h < GameConstants::Board::H; h++)
    {
        for (int w = 0; w < GameConstants::Board::W; w++)
        {
            if (this->walls_[h][w] == 1)
            {
                ss << '#';
            }
            else if (this->character_.y_ == h && this->character_.x_ == w)
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

// 기존 BFS 기반 최단 거리 계산 (이전 코드와 호환성 유지)
int WallMazeState::getDistanceToNearestPoint() const
{
    struct DistanceCoord
    {
        int y_;
        int x_;
        int distance_;
        DistanceCoord() : y_(0), x_(0), distance_(0) {}
        DistanceCoord(const int y, const int x, const int distance) : y_(y), x_(x), distance_(distance) {}
        DistanceCoord(const Coord &coord) : y_(coord.y_), x_(coord.x_), distance_(0) {}
    };

    auto que = std::deque<DistanceCoord>();
    que.emplace_back(this->character_);
    std::vector<std::vector<bool>> check(GameConstants::Board::H, std::vector<bool>(GameConstants::Board::W, false));
    
    while (!que.empty())
    {
        const auto &tmp_cod = que.front();
        que.pop_front();
        if (this->points_[tmp_cod.y_][tmp_cod.x_] > 0)
        {
            return tmp_cod.distance_;
        }
        check[tmp_cod.y_][tmp_cod.x_] = true;

        for (int action = 0; action < 4; action++)
        {
            int ty = tmp_cod.y_ + GameConstants::DY[action];
            int tx = tmp_cod.x_ + GameConstants::DX[action];

            if (GameUtil::isValidCoord<GameConstants::Board::H, GameConstants::Board::W>(Coord(ty, tx)) && 
                !this->walls_[ty][tx] && !check[ty][tx])
            {
                que.emplace_back(ty, tx, tmp_cod.distance_ + 1);
            }
        }
    }
    return GameConstants::Board::H * GameConstants::Board::W; // 도달 불가능한 경우
}

GameConstants::ScoreType WallMazeState::evaluateScore()
{
    // 평가 함수 개선: 현재 점수에 거리 정보를 고려
    this->evaluated_score_ = this->game_score_ * GameConstants::Board::H * GameConstants::Board::W - getDistanceToNearestPoint();
    return this->evaluated_score_;
}

bool WallMazeState::operator<(const WallMazeState& other) const
{
    return this->evaluated_score_ < other.evaluated_score_;
}

int WallMazeState::calculateDistance(const Coord& start, const Coord& goal) const {
    return getDistanceToPoint(goal, pathAlgorithmType_);
}

int WallMazeState::getDistanceToPoint(const Coord& target, PathfindingConstants::Algorithm algo) const {
    auto pathfinder = createPathfindingAlgorithm(algo);
    
    auto isWalkableFunc = [this](int y, int x) -> bool {
        return this->isWalkable(y, x);
    };
    
    PathfindingResult result = pathfinder->findPath(this->character_, target, isWalkableFunc);
    
    return result.pathFound ? result.distance : GameConstants::INF;
}

int WallMazeState::getNextActionTowards(const Coord& target, PathfindingConstants::Algorithm algo) const {
    auto pathfinder = createPathfindingAlgorithm(algo);
    
    auto isWalkableFunc = [this](int y, int x) -> bool {
        return this->isWalkable(y, x);
    };
    
    PathfindingResult result = pathfinder->findPath(this->character_, target, isWalkableFunc);
    
    if (!result.pathFound || result.path.size() < 2) {
        // 경로를 찾지 못했거나, 이미 목표 위치에 있는 경우
        return -1;
    }
    
    // 다음 이동할 위치 (경로의 두 번째 노드, 첫 번째는 현재 위치)
    Coord nextPos = result.path[1];
    
    for (int dir = 0; dir < 4; dir++) {
        int ny = this->character_.y_ + GameConstants::DY[dir];
        int nx = this->character_.x_ + GameConstants::DX[dir];
        
        if (ny == nextPos.y_ && nx == nextPos.x_) {
            return dir;
        }
    }
    
    return -1;
}

Coord WallMazeState::findNearestPoint(PathfindingConstants::Algorithm algo) const {
    Coord nearestPoint(-1, -1);
    int minDistance = GameConstants::INF;
    
    for (int y = 0; y < GameConstants::Board::H; y++) {
        for (int x = 0; x < GameConstants::Board::W; x++) {
            if (this->points_[y][x] > 0) {
                Coord pointPos(y, x);
                int distance = getDistanceToPoint(pointPos, algo);
                
                if (distance < minDistance) {
                    minDistance = distance;
                    nearestPoint = pointPos;
                }
            }
        }
    }
    
    return nearestPoint;
}

Coord WallMazeState::findHighestValuePoint(PathfindingConstants::Algorithm algo) const {
    Coord bestPoint(-1, -1);
    double bestValue = -1;
    
    for (int y = 0; y < GameConstants::Board::H; y++) {
        for (int x = 0; x < GameConstants::Board::W; x++) {
            if (this->points_[y][x] > 0) {
                Coord pointPos(y, x);
                int distance = getDistanceToPoint(pointPos, algo);
                
                if (distance < GameConstants::INF) {
                    // 점수/거리 비율 계산 (거리당 얻는 점수)
                    double value = static_cast<double>(this->points_[y][x]) / distance;
                    
                    if (value > bestValue) {
                        bestValue = value;
                        bestPoint = pointPos;
                    }
                }
            }
        }
    }
    
    return bestPoint;
}

// 경로 탐색 알고리즘 벤치마크 실행
PathfindingResult WallMazeState::benchmarkPathfinding(
    const WallMazeState& state,
    const Coord& start,
    const Coord& goal,
    PathfindingConstants::Algorithm algo
) {
    auto pathfinder = createPathfindingAlgorithm(algo);
    
    auto isWalkableFunc = [&state](int y, int x) -> bool {
        return state.isWalkable(y, x);
    };
    
    return pathfinder->findPath(start, goal, isWalkableFunc);
}