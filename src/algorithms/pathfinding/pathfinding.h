#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "../../common/coord.h"
#include "../../common/game_constants.h"
#include <vector>
#include <functional>
#include <unordered_map>
#include <utility>
#include <string>
#include <memory>

namespace PathfindingConstants {
    enum class Algorithm {
        BFS,
        DFS,
        DIJKSTRA,
        ASTAR,
    };

    const std::unordered_map<Algorithm, std::string> AlgorithmNames = {
        {Algorithm::BFS, "BFS"},
        {Algorithm::DFS, "DFS"},
        {Algorithm::DIJKSTRA, "Dijkstra"},
        {Algorithm::ASTAR, "A*"},
    };
}

struct PathfindingResult {
    std::vector<Coord> path; 
    int distance;
    int nodesExplored;
    double executionTimeMs;
    bool pathFound;

    // 기본 생성자
    PathfindingResult() : distance(0), nodesExplored(0), executionTimeMs(0.0), pathFound(false) {}
};

class PathfindingAlgorithm {
public:

    virtual ~PathfindingAlgorithm() = default;

    virtual PathfindingResult findPath(
        const Coord& start,
        const Coord& goal,
        const std::function<bool(int, int)>& isWalkable
    ) = 0;

    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
};

// 기존 방식의 팩토리 함수 - 탐색 알고리즘 인스턴스 생성
std::unique_ptr<PathfindingAlgorithm> createPathfindingAlgorithm(PathfindingConstants::Algorithm type);

#endif // PATHFINDING_H