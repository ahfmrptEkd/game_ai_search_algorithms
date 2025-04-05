#ifndef BFS_H
#define BFS_H

#include "pathfinding.h"
#include <memory>

class BFSAlgorithm : public PathfindingAlgorithm {
public:
    PathfindingResult findPath(
        const Coord& start,
        const Coord& goal,
        const std::function<bool(int, int)>& isWalkable
    ) override;
    
    std::string getName() const override {
        return "Breadth-First Search";
    }
    
    std::string getDescription() const override {
        return "A graph traversal algorithm that explores all neighbors at the present depth "
               "before moving on to nodes at the next depth level. "
               "Guarantees the shortest path in unweighted graphs.";
    }
};

#endif // BFS_H