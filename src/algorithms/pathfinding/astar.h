#ifndef ASTAR_H
#define ASTAR_H

#include "pathfinding.h"
#include <memory>
#include <cmath>

class AStarAlgorithm : public PathfindingAlgorithm {
enum class Heuristic {
        MANHATTAN,  // 맨해튼 거리 (상하좌우로만 이동 가능한 그리드에 적합)
        EUCLIDEAN,  // 유클리드 거리 (대각선 이동이 가능한 경우에 적합)
        CHEBYSHEV   // 체비셰프 거리 (8방향 이동이 가능한 경우에 적합)
    };
private:
    Heuristic heuristicType_;
    
    int manhattanDistance(const Coord& a, const Coord& b) const {
        return std::abs(a.x_ - b.x_) + std::abs(a.y_ - b.y_);
    }
    
    double euclideanDistance(const Coord& a, const Coord& b) const {
        return std::sqrt(std::pow(a.x_ - b.x_, 2) + std::pow(a.y_ - b.y_, 2));
    }
    
    int chebyshevDistance(const Coord& a, const Coord& b) const {
        return std::max(std::abs(a.x_ - b.x_), std::abs(a.y_ - b.y_));
    }
    
    double heuristic(const Coord& a, const Coord& b) const {
        switch (heuristicType_) {
            case Heuristic::MANHATTAN:
                return manhattanDistance(a, b);
            case Heuristic::EUCLIDEAN:
                return euclideanDistance(a, b);
            case Heuristic::CHEBYSHEV:
                return chebyshevDistance(a, b);
            default:
                return manhattanDistance(a, b);
        }
    }
public:
    AStarAlgorithm(Heuristic heuristicType = Heuristic::MANHATTAN) 
        : heuristicType_(heuristicType) {}
    
    PathfindingResult findPath(
        const Coord& start,
        const Coord& goal,
        const std::function<bool(int, int)>& isWalkable
    ) override;
    
    std::string getName() const override {
        return "A* (A-Star)";
    }
    
    std::string getDescription() const override {
        return "A best-first search algorithm that uses a heuristic function to estimate "
               "the cost from the current node to the goal. Guarantees the shortest path "
               "when using an admissible heuristic.";
    }
};

#endif // ASTAR_H