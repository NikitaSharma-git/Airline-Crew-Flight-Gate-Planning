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

struct WeightedEdge {
    int from;
    int to;
    int weight;
};

struct MSTResult {
    std::vector<WeightedEdge> edges;
    int totalWeight;
    int components;
};

inline MSTResult primMinimumSpanningForest(
    int vertexCount, const std::vector<WeightedEdge>& edges) {
    const int infinity = std::numeric_limits<int>::max() / 4;
    std::vector<std::vector<int>> graph(
        vertexCount, std::vector<int>(vertexCount, 0));

    for (const auto& edge : edges) {
        if (edge.from >= 0 && edge.from < vertexCount &&
            edge.to >= 0 && edge.to < vertexCount && edge.weight > 0) {
            if (graph[edge.from][edge.to] == 0 ||
                edge.weight < graph[edge.from][edge.to]) {
                graph[edge.from][edge.to] = edge.weight;
                graph[edge.to][edge.from] = edge.weight;
            }
        }
    }

    MSTResult result{{}, 0, 0};
    std::vector<int> key(vertexCount, infinity);
    std::vector<int> parent(vertexCount, -1);
    std::vector<bool> inTree(vertexCount, false);

    for (int start = 0; start < vertexCount; ++start) {
        if (inTree[start]) continue;

        ++result.components;
        key[start] = 0;

        for (int count = 0; count < vertexCount; ++count) {
            int current = -1;
            for (int vertex = 0; vertex < vertexCount; ++vertex) {
                if (!inTree[vertex] &&
                    (current == -1 || key[vertex] < key[current])) {
                    current = vertex;
                }
            }

            if (current == -1 || key[current] == infinity) break;
            inTree[current] = true;

            if (parent[current] != -1) {
                result.edges.push_back({parent[current], current, key[current]});
                result.totalWeight += key[current];
            }

            for (int next = 0; next < vertexCount; ++next) {
                int weight = graph[current][next];
                if (weight > 0 && !inTree[next] && weight < key[next]) {
                    key[next] = weight;
                    parent[next] = current;
                }
            }
        }
    }

    return result;
}

class DisjointSet {
private:
    std::vector<int> parent;
    std::vector<int> rankValue;

public:
    explicit DisjointSet(int size) : parent(size), rankValue(size, 0) {
        for (int i = 0; i < size; ++i) parent[i] = i;
    }

    int find(int vertex) {
        if (parent[vertex] != vertex) parent[vertex] = find(parent[vertex]);
        return parent[vertex];
    }

    bool unite(int first, int second) {
        int rootFirst = find(first);
        int rootSecond = find(second);
        if (rootFirst == rootSecond) return false;

        if (rankValue[rootFirst] < rankValue[rootSecond]) {
            parent[rootFirst] = rootSecond;
        } else if (rankValue[rootFirst] > rankValue[rootSecond]) {
            parent[rootSecond] = rootFirst;
        } else {
            parent[rootSecond] = rootFirst;
            ++rankValue[rootFirst];
        }
        return true;
    }
};

inline MSTResult kruskalMinimumSpanningForest(
    int vertexCount, std::vector<WeightedEdge> edges) {
    // Manual insertion sort by edge weight.
    for (std::size_t i = 1; i < edges.size(); ++i) {
        WeightedEdge current = edges[i];
        int j = static_cast<int>(i) - 1;
        while (j >= 0 && edges[j].weight > current.weight) {
            edges[j + 1] = edges[j];
            --j;
        }
        edges[j + 1] = current;
    }

    DisjointSet sets(vertexCount);
    MSTResult result{{}, 0, vertexCount};

    for (const auto& edge : edges) {
        if (edge.from < 0 || edge.from >= vertexCount ||
            edge.to < 0 || edge.to >= vertexCount || edge.weight <= 0) {
            continue;
        }

        if (sets.unite(edge.from, edge.to)) {
            result.edges.push_back(edge);
            result.totalWeight += edge.weight;
            --result.components;
        }
    }

    return result;
}

#endif
