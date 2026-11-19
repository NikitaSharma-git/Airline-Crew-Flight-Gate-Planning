#ifndef AIRLINE_GRAPH_ALGORITHMS_HPP
#define AIRLINE_GRAPH_ALGORITHMS_HPP

#include <limits>
#include <vector>

struct DijkstraResult {
    std::vector<int> distance;
    std::vector<int> parent;
};

inline DijkstraResult dijkstraShortestPath(const std::vector<std::vector<int>>& graph, int source) {
    const int n = static_cast<int>(graph.size());
    const int infinity = std::numeric_limits<int>::max() / 4;

    DijkstraResult result{std::vector<int>(n, infinity), std::vector<int>(n, -1)};
    std::vector<bool> visited(n, false);
    result.distance[source] = 0;

    // Select the unvisited vertex with the smallest known distance.
    for (int count = 0; count < n; ++count) {
        int current = -1;
        for (int vertex = 0; vertex < n; ++vertex) {
            if (!visited[vertex] &&
                (current == -1 || result.distance[vertex] < result.distance[current])) {
                current = vertex;
            }
        }

        if (current == -1 || result.distance[current] == infinity) break;
        visited[current] = true;

        for (int next = 0; next < n; ++next) {
            if (graph[current][next] > 0 && !visited[next] &&
                result.distance[current] + graph[current][next] < result.distance[next]) {
                result.distance[next] = result.distance[current] + graph[current][next];
                result.parent[next] = current;
            }
        }
    }

    return result;
}

#endif
