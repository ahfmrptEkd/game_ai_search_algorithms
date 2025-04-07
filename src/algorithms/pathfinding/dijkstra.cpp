#include "dijkstra.h"
#include "../../common/game_util.h"
#include <queue>
#include <unordered_map>
#include <chrono>

struct DijkstraNode {
    Coord pos;       // 현재 위치
    int distance;    // 시작점에서 현재 노드까지의 거리
    
    // 비교 연산자 (우선순위 큐에서 사용)
    bool operator>(const DijkstraNode& other) const {
        return distance > other.distance;  // 거리가 작은 것이 우선
    }
};

PathfindingResult DijkstraAlgorithm::findPath(
    const Coord& start,
    const Coord& goal,
    const std::function<bool(int, int)>& isWalkable
) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    PathfindingResult result;
    
    // 방문한 노드 추적
    std::vector<std::vector<bool>> visited(
        GameConstants::Board::H, 
        std::vector<bool>(GameConstants::Board::W, false)
    );
    
    // 거리 기록
    std::vector<std::vector<int>> distance(
        GameConstants::Board::H, 
        std::vector<int>(GameConstants::Board::W, GameConstants::INF)
    );
    
    // 이전 노드 추적 (경로 재구성을 위해)
    std::unordered_map<int, Coord> cameFrom;
    
    std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, std::greater<DijkstraNode>> pq;
    
    distance[start.y_][start.x_] = 0;
    pq.push({start, 0});
    
    int nodesExplored = 0;
    
    while (!pq.empty()) {
        DijkstraNode current = pq.top();
        pq.pop();
        nodesExplored++;
        
        if (visited[current.pos.y_][current.pos.x_]) {
            continue;
        }
        
        visited[current.pos.y_][current.pos.x_] = true;
        
        // 목표 도달 시 종료
        if (current.pos.y_ == goal.y_ && current.pos.x_ == goal.x_) {
            result.distance = current.distance;
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
                
                int edgeCost = cost(current.pos.y_, current.pos.x_, ny, nx);
                int newDistance = current.distance + edgeCost;
                
                // 더 짧은 경로를 발견한 경우
                if (newDistance < distance[ny][nx]) {
                    // 이전 노드 기록
                    cameFrom[ny * GameConstants::Board::W + nx] = current.pos;
                    
                    // 거리 업데이트
                    distance[ny][nx] = newDistance;
                    
                    // 이 이웃을 큐에 추가
                    pq.push({Coord(ny, nx), newDistance});
                }
            }
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    
    result.nodesExplored = nodesExplored;
    result.executionTimeMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    
    return result;
}