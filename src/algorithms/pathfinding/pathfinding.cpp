#include "pathfinding.h"
#include "bfs.h"
#include "dfs.h"
#include <stdexcept>

std::unique_ptr<PathfindingAlgorithm> createPathfindingAlgorithm(PathfindingConstants::Algorithm type) {
    switch (type) {
        case PathfindingConstants::Algorithm::BFS:
            return std::make_unique<BFSAlgorithm>();
        case PathfindingConstants::Algorithm::DFS:
            return std::make_unique<DFSAlgorithm>();
        
        default:
            throw std::invalid_argument("Unknown pathfinding algorithm type");
    }
}