#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "pathfinding.h"
#include <memory>
#include <functional>

class DijkstraAlgorithm : public PathfindingAlgorithm {
private:
    std::function<int(int, int, int, int)> costFunction_;
    
    int defaultCost(int fromY, int fromX, int toY, int toX) const {
        return 1;
    }
    
    // 실제 비용 계산
    int cost(int fromY, int fromX, int toY, int toX) const {
        if (costFunction_) {
            return costFunction_(fromY, fromX, toY, toX);
        }
        return defaultCost(fromY, fromX, toY, toX);
    }
public:
    DijkstraAlgorithm(std::function<int(int, int, int, int)> costFunction = nullptr) 
        : costFunction_(costFunction) {}
    
    PathfindingResult findPath(
        const Coord& start,
        const Coord& goal,
        const std::function<bool(int, int)>& isWalkable
    ) override;
    
    std::string getName() const override {
        return "Dijkstra";
    }
    
    std::string getDescription() const override {
        return "A graph search algorithm that finds the shortest paths between nodes in a graph. "
               "Similar to A* but without a heuristic function, Dijkstra's algorithm "
               "guarantees the shortest path in weighted graphs.";
    }
};

#endif // DIJKSTRA_H