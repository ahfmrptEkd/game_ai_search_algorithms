#include "astar.h"
#include "../../common/game_util.h"
#include <queue>
#include <unordered_map>
#include <chrono>
#include <cmath>

struct AStarNode {
    Coord pos;           
    int g;               
    double f;            
    
    // 비교 연산자 (우선순위 큐에서 사용)
    bool operator>(const AStarNode& other) const {
        return f > other.f;  // f값이 작은 것이 우선
    }
};

PathfindingResult AStarAlgorithm::findPath(
    const Coord& start,
    const Coord& goal,
    const std::function<bool(int, int)>& isWalkable
) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    PathfindingResult result;
    
    std::vector<std::vector<bool>> visited(
        GameConstants::Board::H, 
        std::vector<bool>(GameConstants::Board::W, false)
    );
    
    std::vector<std::vector<int>> gScore(
        GameConstants::Board::H, 
        std::vector<int>(GameConstants::Board::W, GameConstants::INF)
    );
    
    std::unordered_map<int, Coord> cameFrom;
    
    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> openSet;
    
    // 시작 노드 초기화
    gScore[start.y_][start.x_] = 0;
    openSet.push({start, 0, heuristic(start, goal)});
    
    int nodesExplored = 0;
    
    while (!openSet.empty()) {
        AStarNode current = openSet.top();
        openSet.pop();
        nodesExplored++;
        
        if (visited[current.pos.y_][current.pos.x_]) {
            continue;
        }
        
        visited[current.pos.y_][current.pos.x_] = true;
        
        if (current.pos.y_ == goal.y_ && current.pos.x_ == goal.x_) {
            result.distance = current.g;
            result.pathFound = true;
            
            Coord backtrack = current.pos;
            while (!(backtrack.y_ == start.y_ && backtrack.x_ == start.x_)) {
                result.path.push_back(backtrack);
                int key = backtrack.y_ * GameConstants::Board::W + backtrack.x_;
                backtrack = cameFrom[key];
            }
            result.path.push_back(start);
            
            std::reverse(result.path.begin(), result.path.end());
            break;
        }
        
        for (int dir = 0; dir < 4; dir++) {
            int ny = current.pos.y_ + GameConstants::DY[dir];
            int nx = current.pos.x_ + GameConstants::DX[dir];
            
            if (GameUtil::isValidCoord<GameConstants::Board::H, GameConstants::Board::W>(Coord(ny, nx)) && 
                isWalkable(ny, nx) && !visited[ny][nx]) {
                
                Coord neighbor(ny, nx);
                
                // 이웃 노드까지의 새 비용 계산
                int tentative_gScore = current.g + 1;  // 모든 이동은 비용 1
                
                // 더 좋은 경로를 발견한 경우
                if (tentative_gScore < gScore[ny][nx]) {
                    // 이전 노드 기록
                    cameFrom[ny * GameConstants::Board::W + nx] = current.pos;
                    
                    // 비용 업데이트
                    gScore[ny][nx] = tentative_gScore;
                    
                    // 이 이웃을 오픈셋에 추가
                    double fScore = tentative_gScore + heuristic(neighbor, goal);
                    openSet.push({neighbor, tentative_gScore, fScore});
                }
            }
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    
    result.nodesExplored = nodesExplored;
    result.executionTimeMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    
    return result;
}