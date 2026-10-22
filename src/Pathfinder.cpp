
#include "Pathfinder.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

std::vector<int> Pathfinder::neighbors(int node, int rows, int cols) {
    const int r = node / cols;
    const int c = node % cols;

    std::vector<int> result;
    result.reserve(4);

    if (r > 0) result.push_back(node - cols);
    if (r + 1 < rows) result.push_back(node + cols);
    if (c > 0) result.push_back(node - 1);
    if (c + 1 < cols) result.push_back(node + 1);

    return result;
}

int Pathfinder::heuristic(int a, int b, int cols) {
    const int ar = a / cols;
    const int ac = a % cols;
    const int br = b / cols;
    const int bc = b % cols;

    // Manhattan distance: suitable for four-directional grid movement.
    return std::abs(ar - br) + std::abs(ac - bc);
}

SearchResult Pathfinder::search(
    int rows,
    int cols,
    const std::vector<bool>& walls,
    int start,
    int goal,
    Algorithm algorithm
) {
    SearchResult result;

    // Validate the grid before performing any search.
    if (rows <= 0 || cols <= 0) {
        return result;
    }

    const int total = rows * cols;

    if (start < 0 || goal < 0 ||
        start >= total || goal >= total ||
        walls.size() != static_cast<std::size_t>(total) ||
        walls[start] || walls[goal]) {
        return result;
    }

    const auto searchStart = std::chrono::steady_clock::now();

    const int INF = std::numeric_limits<int>::max() / 4;

    std::vector<int> parent(total, -1);
    std::vector<int> distance(total, INF);
    std::vector<bool> closed(total, false);

    if (algorithm == Algorithm::BFS) {
        // BFS uses a FIFO queue.
        std::queue<int> frontier;

        distance[start] = 0;
        frontier.push(start);

        while (!frontier.empty()) {
            const int current = frontier.front();
            frontier.pop();

            if (closed[current]) {
                continue;
            }

            closed[current] = true;
            result.visitedOrder.push_back(current);

            if (current == goal) {
                break;
            }

            for (const int next : neighbors(current, rows, cols)) {
                if (walls[next] || closed[next] ||
                    distance[next] != INF) {
                    continue;
                }

                distance[next] = distance[current] + 1;
                parent[next] = current;
                frontier.push(next);
            }
        }
    } else if (algorithm == Algorithm::DFS) {
        // DFS uses a LIFO stack.
        std::vector<int> frontier;

        distance[start] = 0;
        frontier.push_back(start);

        while (!frontier.empty()) {
            const int current = frontier.back();
            frontier.pop_back();

            if (closed[current]) {
                continue;
            }

            closed[current] = true;
            result.visitedOrder.push_back(current);

            if (current == goal) {
                break;
            }

            for (const int next : neighbors(current, rows, cols)) {
                if (walls[next] || closed[next] ||
                    distance[next] != INF) {
                    continue;
                }

                distance[next] = distance[current] + 1;
                parent[next] = current;
                frontier.push_back(next);
            }
        }
    } else if (algorithm == Algorithm::Dijkstra ||
               algorithm == Algorithm::AStar) {
        // Each entry contains {priority, node}.
        using Entry = std::pair<int, int>;

        std::priority_queue<
            Entry,
            std::vector<Entry>,
            std::greater<Entry>
        > frontier;

        distance[start] = 0;

        const int initialPriority =
            algorithm == Algorithm::AStar
                ? heuristic(start, goal, cols)
                : 0;

        frontier.push({initialPriority, start});

        while (!frontier.empty()) {
            const int current = frontier.top().second;
            frontier.pop();

            if (closed[current]) {
                continue;
            }

            closed[current] = true;
            result.visitedOrder.push_back(current);

            if (current == goal) {
                break;
            }

            for (const int next : neighbors(current, rows, cols)) {
                if (walls[next] || closed[next]) {
                    continue;
                }

                const int candidate = distance[current] + 1;

                if (candidate >= distance[next]) {
                    continue;
                }

                distance[next] = candidate;
                parent[next] = current;

                const int priority =
                    candidate +
                    (algorithm == Algorithm::AStar
                        ? heuristic(next, goal, cols)
                        : 0);

                frontier.push({priority, next});
            }
        }
    } else {
        // Unknown algorithm value.
        return result;
    }

    // Record metrics for the search itself, before reconstructing the path.
    const auto searchEnd = std::chrono::steady_clock::now();

    result.executionTimeMs =
        std::chrono::duration<double, std::milli>(
            searchEnd - searchStart
        ).count();

    result.exploredCount = result.visitedOrder.size();

    // Reconstruct the path only if the goal was reached.
    if (closed[goal]) {
        result.found = true;

        int current = goal;

        while (current != start) {
            result.path.push_back(current);
            current = parent[current];

            if (current < 0) {
                result.path.clear();
                result.found = false;
                result.pathCost = 0;
                return result;
            }
        }

        result.path.push_back(start);

        std::reverse(result.path.begin(), result.path.end());

        result.pathCost =
            static_cast<int>(result.path.size()) - 1;
    }

    return result;
}
