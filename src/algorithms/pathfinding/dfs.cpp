#include "dfs.h"
#include "../../common/game_util.h"
#include <stack>
#include <unordered_map>
#include <chrono>

PathfindingResult DFSAlgorithm::findPath(
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
    
    // 이전 노드 추적 (경로 재구성을 위해)
    std::unordered_map<int, Coord> cameFrom;
    
    std::stack<std::pair<Coord, int>> stack;
    
    stack.push({start, 0});
    visited[start.y_][start.x_] = true;
    
    int nodesExplored = 0;
    
    while (!stack.empty()) {
        auto [current, distance] = stack.top();
        stack.pop();
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
        
        for (int dir = 3; dir >= 0; dir--) {
            int ny = current.y_ + GameConstants::DY[dir];
            int nx = current.x_ + GameConstants::DX[dir];
            
            if (GameUtil::isValidCoord<GameConstants::Board::H, GameConstants::Board::W>(Coord(ny, nx)) && 
                isWalkable(ny, nx) && !visited[ny][nx]) {
                
                Coord next(ny, nx);
                visited[ny][nx] = true;
                
                // 이전 노드 기록
                cameFrom[ny * GameConstants::Board::W + nx] = current;
                
                stack.push({next, distance + 1});
            }
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    
    // 결과 설정
    result.nodesExplored = nodesExplored;
    result.executionTimeMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    
    return result;
}

bool DFSAlgorithm::dfsRecursive(
    const Coord& current,
    const Coord& goal,
    std::vector<std::vector<bool>>& visited,
    std::unordered_map<int, Coord>& cameFrom,
    const std::function<bool(int, int)>& isWalkable,
    int& nodesExplored
) {
    nodesExplored++;
    
    if (current.y_ == goal.y_ && current.x_ == goal.x_) {
        return true;
    }
    
    for (int dir = 0; dir < 4; dir++) {
        int ny = current.y_ + GameConstants::DY[dir];
        int nx = current.x_ + GameConstants::DX[dir];
        
        if (GameUtil::isValidCoord<GameConstants::Board::H, GameConstants::Board::W>(Coord(ny, nx)) && 
            isWalkable(ny, nx) && !visited[ny][nx]) {
            
            Coord next(ny, nx);
            visited[ny][nx] = true;
            
            cameFrom[ny * GameConstants::Board::W + nx] = current;
            
            if (dfsRecursive(next, goal, visited, cameFrom, isWalkable, nodesExplored)) {
                return true;
            }
        }
    }
    
    return false;
}