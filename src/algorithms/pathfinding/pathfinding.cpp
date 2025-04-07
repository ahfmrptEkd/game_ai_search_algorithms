#include "pathfinding.h"
#include "bfs.h"
#include "dfs.h"
#include "dijkstra.h"
#include "astar.h"
#include <stdexcept>

std::unique_ptr<PathfindingAlgorithm> createPathfindingAlgorithm(PathfindingConstants::Algorithm type) {
    switch (type) {
        case PathfindingConstants::Algorithm::BFS:
            return std::make_unique<BFSAlgorithm>();
        case PathfindingConstants::Algorithm::DFS:
            return std::make_unique<DFSAlgorithm>();
        case PathfindingConstants::Algorithm::DIJKSTRA:
            return std::make_unique<DijkstraAlgorithm>();
        case PathfindingConstants::Algorithm::ASTAR:
            return std::make_unique<AStarAlgorithm>();
        default:
            throw std::invalid_argument("Unknown pathfinding algorithm type");
    }
}