#include "wallmaze_state.h"
#include "../../common/game_util.h"
#include <iostream>
#include <sstream>
#include <random>
#include <queue>
WallMazeState::WallMazeState() {}

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
}

bool WallMazeState::isDone() const
{
    return this->turn_ == GameConstants::Board::END_TURN;
}

void WallMazeState::progress(const int action)
{
    this->character_.x_ += GameConstants::DX[action];
    this->character_.y_ += GameConstants::DY[action];
    auto &point = this->points_[this->character_.y_][this->character_.x_];
    if (point > 0)
    {
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

GameConstants::ScoreType WallMazeState::evaluateScore()
{
    ScoreType baseScore = static_cast<GameConstants::ScoreType>(this->game_score_);
    
    ScoreType potentialScore = evaluatePotentialScore();
    
    // 최종 평가 점수 = 현재 점수 + 가중치를 둔 잠재적 점수
    // 가중치는 0.8로 설정 (현재 점수보다 약간 낮은 중요도)
    this->evaluated_score_ = baseScore + static_cast<ScoreType>(potentialScore * 0.8);
    
    return this->evaluated_score_;
}

bool WallMazeState::operator<(const WallMazeState& other) const
{
    return this->evaluated_score_ < other.evaluated_score_;
}

int WallMazeState::bfsDistance(const Coord& start, const Coord& goal) const
{
    bool visited[GameConstants::Board::H][GameConstants::Board::W] = {};
    
    // BFS 큐 - <좌표, 거리> 쌍을 저장
    std::queue<std::pair<Coord, int>> queue;
    
    queue.push({start, 0});
    visited[start.y_][start.x_] = true;
    
    while (!queue.empty()) {
        auto [current, distance] = queue.front();
        queue.pop();
        
        if (current.y_ == goal.y_ && current.x_ == goal.x_) {
            return distance;
        }
        
        for (int dir = 0; dir < 4; dir++) {
            int ny = current.y_ + GameConstants::DY[dir];
            int nx = current.x_ + GameConstants::DX[dir];
            
            if (GameUtil::isValidCoord<GameConstants::Board::H, GameConstants::Board::W>(Coord(ny, nx)) && 
                !this->hasWall(ny, nx) && !visited[ny][nx]) {
                
                visited[ny][nx] = true;
                queue.push({Coord(ny, nx), distance + 1});
            }
        }
    }
    
    return GameConstants::INF;
}

// 잠재적 점수 평가 (남은 턴으로 얻을 수 있는 최대 점수 예측)
ScoreType WallMazeState::evaluatePotentialScore() const
{
    int remainingTurns = GameConstants::Board::END_TURN - this->turn_;
    if (remainingTurns <= 0) return 0;
    
    // 모든 점수 위치와 거리 정보 수집
    std::vector<std::tuple<int, int, Coord>> pointData; // <거리, 점수, 위치>
    
    for (int y = 0; y < GameConstants::Board::H; y++) {
        for (int x = 0; x < GameConstants::Board::W; x++) {
            if (this->points_[y][x] > 0) {
                Coord pos(y, x);
                int distance = bfsDistance(this->character_, pos);
                if (distance < GameConstants::INF) { // 도달 가능한 경우만
                    pointData.push_back({distance, this->points_[y][x], pos});
                }
            }
        }
    }
    
    std::sort(pointData.begin(), pointData.end());
    
    ScoreType potentialScore = 0;
    int turnsUsed = 0;
    Coord currentPos = this->character_;
    
    for (size_t i = 0; i < pointData.size(); i++) {
        const auto& [_, value, pos] = pointData[i];
        
        int distance = bfsDistance(currentPos, pos);
        
        if (turnsUsed + distance <= remainingTurns) {
            potentialScore += value;
            turnsUsed += distance;
            currentPos = pos;
            
            // 이미 처리한 점수는 제외하고 남은 점수들에 대해 재계산
            // (구현 간소화를 위해 생략되었지만, 보다 정확한 예측을 위해 필요)
        } else {
            break; // 남은 턴으로 도달할 수 없음
        }
    }
    
    return potentialScore;
}