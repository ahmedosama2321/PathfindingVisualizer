
#pragma once

#include <cstddef>
#include <vector>

enum class Algorithm {
    BFS,
    DFS,
    Dijkstra,
    AStar
};

struct SearchResult {
    // Nodes visited during the search
    std::vector<int> visitedOrder;

    // Final path from start to goal
    std::vector<int> path;

    // Search status
    bool found = false;

    // Total cost of the final path
    int pathCost = 0;

    // Number of nodes explored
    std::size_t exploredCount = 0;

    // Search execution time in milliseconds
    double executionTimeMs = 0.0;
};

class Pathfinder {
public:
    static SearchResult search(
        int rows,
        int cols,
        const std::vector<bool>& walls,
        int start,
        int goal,
        Algorithm algorithm
    );

private:
    static std::vector<int> neighbors(
        int node,
        int rows,
        int cols
    );

    static int heuristic(
        int a,
        int b,
        int cols
    );
};
