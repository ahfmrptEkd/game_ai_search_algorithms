#include "pathfinding.h"
#include "bfs.h"
#include <stdexcept>

std::unique_ptr<PathfindingAlgorithm> createPathfindingAlgorithm(PathfindingConstants::Algorithm type) {
    switch (type) {
        case PathfindingConstants::Algorithm::BFS:
            return std::make_unique<BFSAlgorithm>();
        
        default:
            throw std::invalid_argument("Unknown pathfinding algorithm type");
    }
}