#include "bfs.h"
#include "../../common/game_util.h"
#include <queue>
#include <unordered_map>
#include <chrono>

PathfindingResult BFSAlgorithm::findPath(
    const Coord& start,
    const Coord& goal,
    const std::function<bool(int, int)>& isWalkable
) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    PathfindingResult result;
    
    bool visited[GameConstants::Board::H][GameConstants::Board::W] = {};
    
    // 이전 노드 추적 (경로 재구성을 위해)
    std::unordered_map<int, Coord> cameFrom;
    
    std::queue<std::pair<Coord, int>> queue;
    
    queue.push({start, 0});
    visited[start.y_][start.x_] = true;
    
    int nodesExplored = 0;
    
    while (!queue.empty()) {
        auto [current, distance] = queue.front();
        queue.pop();
        nodesExplored++;
        
        // 목표 도달 시 종료
        if (current.y_ == goal.y_ && current.x_ == goal.x_) {
            result.distance = distance;
            result.pathFound = true;
            
            Coord backtrack = current;
            while (!(backtrack.y_ == start.y_ && backtrack.x_ == start.x_)) {
                result.path.push_back(backtrack);
                int key = backtrack.y_ * GameConstants::Board::W + backtrack.x_;
                backtrack = cameFrom[key];
            }
            result.path.push_back(start);
            
            // 경로 순서 뒤집기 (시작->목표)
            std::reverse(result.path.begin(), result.path.end());
            break;
        }
        
        for (int dir = 0; dir < 4; dir++) {
            int ny = current.y_ + GameConstants::DY[dir];
            int nx = current.x_ + GameConstants::DX[dir];
            
            if (GameUtil::isValidCoord<GameConstants::Board::H, GameConstants::Board::W>(Coord(ny, nx)) && 
                isWalkable(ny, nx) && !visited[ny][nx]) {
                
                Coord next(ny, nx);
                visited[ny][nx] = true;
                
                cameFrom[ny * GameConstants::Board::W + nx] = current;
                
                queue.push({next, distance + 1});
            }
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    
    result.nodesExplored = nodesExplored;
    result.executionTimeMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    
    return result;
}