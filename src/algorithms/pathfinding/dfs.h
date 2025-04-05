#ifndef DFS_H
#define DFS_H

#include "pathfinding.h"
#include <memory>

class DFSAlgorithm : public PathfindingAlgorithm {
private:
    bool dfsRecursive(
        const Coord& current,
        const Coord& goal,
        std::vector<std::vector<bool>>& visited,
        std::unordered_map<int, Coord>& cameFrom,
        const std::function<bool(int, int)>& isWalkable,
        int& nodesExplored
    );
public:
    PathfindingResult findPath(
        const Coord& start,
        const Coord& goal,
        const std::function<bool(int, int)>& isWalkable
    ) override;
    
    std::string getName() const override {
        return "Depth-First Search";
    }
    
    std::string getDescription() const override {
        return "A graph traversal algorithm that explores as far as possible along each branch "
               "before backtracking. Uses less memory than BFS but does not guarantee "
               "the shortest path in unweighted graphs.";
    }
};

#endif // DFS_H