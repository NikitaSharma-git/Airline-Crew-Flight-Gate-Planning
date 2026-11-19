#ifndef AIRLINE_ADVANCED_ALGORITHMS_HPP
#define AIRLINE_ADVANCED_ALGORITHMS_HPP

#include <algorithm>
#include <climits>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "models.hpp"

namespace advanced {

struct Edge { int from; int to; int weight; };
struct PathResult { std::vector<int> distance; bool hasNegativeCycle; };
struct KnapsackResult { int value; std::vector<int> selected; };
struct BnBResult { int value; std::vector<int> selected; long long explored; };
struct TspResult { int cost; std::vector<int> path; long long explored; };

inline KnapsackResult knapsack01(const std::vector<int>& weights,
                                 const std::vector<int>& values, int capacity) {
    const int n = static_cast<int>(weights.size());
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));
    for (int i = 1; i <= n; ++i)
        for (int c = 0; c <= capacity; ++c) {
            dp[i][c] = dp[i - 1][c];
            if (weights[i - 1] <= c)
                dp[i][c] = std::max(dp[i][c], values[i - 1] + dp[i - 1][c - weights[i - 1]]);
        }
    KnapsackResult result{dp[n][capacity], {}};
    int c = capacity;
    for (int i = n; i > 0; --i)
        if (dp[i][c] != dp[i - 1][c]) {
            result.selected.push_back(i - 1);
            c -= weights[i - 1];
        }
    std::reverse(result.selected.begin(), result.selected.end());
    return result;
}

inline int matrixChainMinimumCost(const std::vector<int>& dimensions) {
    int n = static_cast<int>(dimensions.size()) - 1;
    if (n <= 1) return 0;
    std::vector<std::vector<long long>> dp(n, std::vector<long long>(n, 0));
    for (int length = 2; length <= n; ++length)
        for (int i = 0; i + length <= n; ++i) {
            int j = i + length - 1;
            dp[i][j] = LLONG_MAX;
            for (int k = i; k < j; ++k)
                dp[i][j] = std::min(dp[i][j], dp[i][k] + dp[k + 1][j] +
                    1LL * dimensions[i] * dimensions[k + 1] * dimensions[j + 1]);
        }
    return static_cast<int>(dp[0][n - 1]);
}

inline int longestCommonSubsequence(const std::string& a, const std::string& b) {
    std::vector<std::vector<int>> dp(a.size() + 1, std::vector<int>(b.size() + 1, 0));
    for (std::size_t i = 1; i <= a.size(); ++i)
        for (std::size_t j = 1; j <= b.size(); ++j)
            dp[i][j] = a[i - 1] == b[j - 1] ? 1 + dp[i - 1][j - 1]
                                            : std::max(dp[i - 1][j], dp[i][j - 1]);
    return dp[a.size()][b.size()];
}

inline long long optimalBstCost(const std::vector<int>& frequency) {
    int n = static_cast<int>(frequency.size());
    if (n == 0) return 0;
    std::vector<std::vector<long long>> dp(n, std::vector<long long>(n, 0));
    std::vector<long long> prefix(n + 1, 0);
    for (int i = 0; i < n; ++i) prefix[i + 1] = prefix[i] + frequency[i];
    for (int length = 1; length <= n; ++length)
        for (int i = 0; i + length <= n; ++i) {
            int j = i + length - 1;
            dp[i][j] = LLONG_MAX;
            long long sum = prefix[j + 1] - prefix[i];
            for (int root = i; root <= j; ++root) {
                long long left = root > i ? dp[i][root - 1] : 0;
                long long right = root < j ? dp[root + 1][j] : 0;
                dp[i][j] = std::min(dp[i][j], left + right + sum);
            }
        }
    return dp[0][n - 1];
}

inline PathResult bellmanFord(int n, const std::vector<Edge>& edges, int source) {
    const int inf = INT_MAX / 4;
    std::vector<int> dist(n, inf);
    dist[source] = 0;
    for (int i = 1; i < n; ++i) {
        bool changed = false;
        for (const auto& e : edges)
            if (dist[e.from] != inf && dist[e.from] + e.weight < dist[e.to]) {
                dist[e.to] = dist[e.from] + e.weight;
                changed = true;
            }
        if (!changed) break;
    }
    bool negative = false;
    for (const auto& e : edges)
        if (dist[e.from] != inf && dist[e.from] + e.weight < dist[e.to]) negative = true;
    return {dist, negative};
}

inline std::vector<std::vector<int>> floydWarshall(std::vector<std::vector<int>> dist) {
    const int n = static_cast<int>(dist.size());
    const int inf = INT_MAX / 4;
    for (int k = 0; k < n; ++k)
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (dist[i][k] != inf && dist[k][j] != inf &&
                    dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
    return dist;
}

inline TspResult boundedTsp(const std::vector<std::vector<int>>& graph, int start) {
    int n = static_cast<int>(graph.size());
    const int inf = INT_MAX / 4;
    TspResult best{inf, {}, 0};
    std::vector<bool> used(n, false);
    std::vector<int> path{start};
    used[start] = true;
    std::function<void(int, int)> search = [&](int current, int cost) {
        ++best.explored;
        if (cost >= best.cost) return;
        if (static_cast<int>(path.size()) == n) {
            if (graph[current][start] > 0 && cost + graph[current][start] < best.cost) {
                best.cost = cost + graph[current][start];
                best.path = path;
                best.path.push_back(start);
            }
            return;
        }
        for (int next = 0; next < n; ++next)
            if (!used[next] && graph[current][next] > 0) {
                used[next] = true; path.push_back(next);
                search(next, cost + graph[current][next]);
                path.pop_back(); used[next] = false;
            }
    };
    if (n > 0) search(start, 0);
    return best;
}

inline bool nQueensSolve(int n, std::vector<int>& placement) {
    placement.assign(n, -1);
    std::function<bool(int)> place = [&](int row) {
        if (row == n) return true;
        for (int col = 0; col < n; ++col) {
            bool safe = true;
            for (int prior = 0; prior < row; ++prior)
                if (placement[prior] == col ||
                    std::abs(placement[prior] - col) == row - prior) safe = false;
            if (safe) {
                placement[row] = col;
                if (place(row + 1)) return true;
            }
        }
        placement[row] = -1;
        return false;
    };
    return place(0);
}

inline bool subsetSum(const std::vector<int>& values, int target, std::vector<int>& chosen) {
    chosen.clear();
    std::function<bool(int, int)> search = [&](int index, int remaining) {
        if (remaining == 0) return true;
        if (index == static_cast<int>(values.size()) || remaining < 0) return false;
        chosen.push_back(index);
        if (search(index + 1, remaining - values[index])) return true;
        chosen.pop_back();
        return search(index + 1, remaining);
    };
    return search(0, target);
}

inline BnBResult knapsackBranchBound(const std::vector<int>& weights,
                                     const std::vector<int>& values,
                                     int capacity, int strategy) {
    struct Node { int level, weight, value; std::vector<int> chosen; };
    int n = static_cast<int>(weights.size());
    BnBResult best{0, {}, 0};
    auto bound = [&](const Node& node) {
        if (node.weight >= capacity) return static_cast<double>(node.value);
        double estimate = node.value;
        int room = capacity - node.weight;
        for (int i = node.level + 1; i < n && room > 0; ++i) {
            if (weights[i] <= room) { room -= weights[i]; estimate += values[i]; }
            else { estimate += static_cast<double>(values[i]) * room / weights[i]; room = 0; }
        }
        return estimate;
    };
    std::vector<Node> pending;
    pending.push_back({-1, 0, 0, {}});
    while (!pending.empty()) {
        Node node;
        if (strategy == 0) { node = pending.front(); pending.erase(pending.begin()); }
        else if (strategy == 1) { node = pending.back(); pending.pop_back(); }
        else {
            int bestIndex = 0;
            for (int i = 1; i < static_cast<int>(pending.size()); ++i)
                if (bound(pending[i]) > bound(pending[bestIndex])) bestIndex = i;
            node = pending[bestIndex];
            pending.erase(pending.begin() + bestIndex);
        }
        ++best.explored;
        if (node.level == n - 1 || bound(node) <= best.value) continue;
        int next = node.level + 1;
        Node include = node;
        include.level = next;
        include.weight += weights[next];
        include.value += values[next];
        include.chosen.push_back(next);
        if (include.weight <= capacity && include.value > best.value) {
            best.value = include.value;
            best.selected = include.chosen;
        }
        if (include.weight <= capacity && bound(include) > best.value) pending.push_back(include);
        Node exclude = node;
        exclude.level = next;
        if (bound(exclude) > best.value) pending.push_back(exclude);
    }
    return best;
}

inline int fordFulkerson(std::vector<std::vector<int>> capacity, int source, int sink) {
    int n = static_cast<int>(capacity.size()), flow = 0;
    while (true) {
        std::vector<int> parent(n, -1);
        std::queue<int> q; q.push(source); parent[source] = source;
        while (!q.empty() && parent[sink] == -1) {
            int u = q.front(); q.pop();
            for (int v = 0; v < n; ++v)
                if (parent[v] == -1 && capacity[u][v] > 0) {
                    parent[v] = u; q.push(v);
                }
        }
        if (parent[sink] == -1) break;
        int augment = INT_MAX;
        for (int v = sink; v != source; v = parent[v])
            augment = std::min(augment, capacity[parent[v]][v]);
        for (int v = sink; v != source; v = parent[v]) {
            capacity[parent[v]][v] -= augment;
            capacity[v][parent[v]] += augment;
        }
        flow += augment;
    }
    return flow;
}

inline int maximumBipartiteMatching(const std::vector<std::vector<int>>& adjacency,
                                    int rightCount, std::vector<int>& rightMatch) {
    int leftCount = static_cast<int>(adjacency.size());
    rightMatch.assign(rightCount, -1);
    int matches = 0;
    for (int left = 0; left < leftCount; ++left) {
        std::vector<bool> seen(rightCount, false);
        std::function<bool(int)> augment = [&](int u) {
            for (int v : adjacency[u]) {
                if (v < 0 || v >= rightCount || seen[v]) continue;
                seen[v] = true;
                if (rightMatch[v] == -1 || augment(rightMatch[v])) {
                    rightMatch[v] = u; return true;
                }
            }
            return false;
        };
        if (augment(left)) ++matches;
    }
    return matches;
}

inline long long extendedGcd(long long a, long long b, long long& x, long long& y) {
    if (b == 0) { x = 1; y = 0; return a; }
    long long x1 = 0, y1 = 0;
    long long g = extendedGcd(b, a % b, x1, y1);
    x = y1; y = x1 - (a / b) * y1;
    return g;
}

inline bool chineseRemainder(const std::vector<int>& residues,
                             const std::vector<int>& moduli, long long& answer) {
    if (residues.size() != moduli.size() || residues.empty()) return false;
    long long x = 0, product = 1;
    for (std::size_t i = 0; i < moduli.size(); ++i) {
        if (moduli[i] <= 0) return false;
        long long m = moduli[i], a = ((residues[i] % m) + m) % m;
        long long inv = 0, unused = 0;
        if (extendedGcd(product, m, inv, unused) != 1) return false;
        long long t = ((a - x) % m + m) % m;
        t = (t * ((inv % m + m) % m)) % m;
        x += product * t;
        product *= m;
        x = (x % product + product) % product;
    }
    answer = x;
    return true;
}

inline std::vector<int> naiveSearch(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    if (pattern.empty()) return matches;
    for (int i = 0; i + static_cast<int>(pattern.size()) <= static_cast<int>(text.size()); ++i) {
        int j = 0;
        while (j < static_cast<int>(pattern.size()) && text[i + j] == pattern[j]) ++j;
        if (j == static_cast<int>(pattern.size())) matches.push_back(i);
    }
    return matches;
}

inline std::vector<int> rabinKarpSearch(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    int n = static_cast<int>(text.size()), m = static_cast<int>(pattern.size());
    if (m == 0 || m > n) return matches;
    const long long base = 256, mod = 1000000007;
    long long high = 1, patternHash = 0, windowHash = 0;
    for (int i = 0; i < m - 1; ++i) high = (high * base) % mod;
    for (int i = 0; i < m; ++i) {
        patternHash = (patternHash * base + static_cast<unsigned char>(pattern[i])) % mod;
        windowHash = (windowHash * base + static_cast<unsigned char>(text[i])) % mod;
    }
    for (int i = 0; i <= n - m; ++i) {
        if (patternHash == windowHash && text.compare(i, m, pattern) == 0) matches.push_back(i);
        if (i < n - m) {
            windowHash = (windowHash - static_cast<unsigned char>(text[i]) * high % mod + mod) % mod;
            windowHash = (windowHash * base + static_cast<unsigned char>(text[i + m])) % mod;
        }
    }
    return matches;
}

inline std::vector<int> buildLps(const std::string& pattern) {
    std::vector<int> lps(pattern.size(), 0);
    for (int i = 1, len = 0; i < static_cast<int>(pattern.size());) {
        if (pattern[i] == pattern[len]) lps[i++] = ++len;
        else if (len) len = lps[len - 1];
        else lps[i++] = 0;
    }
    return lps;
}

inline std::vector<int> kmpSearch(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    if (pattern.empty()) return matches;
    auto lps = buildLps(pattern);
    int i = 0, j = 0;
    while (i < static_cast<int>(text.size())) {
        if (text[i] == pattern[j]) { ++i; ++j; }
        if (j == static_cast<int>(pattern.size())) {
            matches.push_back(i - j); j = lps[j - 1];
        } else if (i < static_cast<int>(text.size()) && text[i] != pattern[j]) {
            if (j) j = lps[j - 1]; else ++i;
        }
    }
    return matches;
}

inline std::vector<int> finiteAutomatonSearch(const std::string& text,
                                               const std::string& pattern) {
    // DFA-style matcher: precompute the next state for each byte using the pattern prefix.
    std::vector<int> matches;
    if (pattern.empty()) return matches;
    const int m = static_cast<int>(pattern.size());
    std::vector<std::vector<int>> transition(m + 1, std::vector<int>(256, 0));
    for (int state = 0; state <= m; ++state)
        for (int ch = 0; ch < 256; ++ch) {
            std::string candidate = pattern.substr(0, state) + static_cast<char>(ch);
            int limit = std::min(m, static_cast<int>(candidate.size()));
            while (limit > 0 && candidate.compare(candidate.size() - limit, limit, pattern, 0, limit) != 0) --limit;
            transition[state][ch] = limit;
        }
    int state = 0;
    for (int i = 0; i < static_cast<int>(text.size()); ++i) {
        state = transition[state][static_cast<unsigned char>(text[i])];
        if (state == m) matches.push_back(i - m + 1);
    }
    return matches;
}

struct HuffmanNode {
    char symbol; int frequency; HuffmanNode* left; HuffmanNode* right;
    HuffmanNode(char c, int f) : symbol(c), frequency(f), left(nullptr), right(nullptr) {}
};

inline void buildCodes(HuffmanNode* node, const std::string& prefix,
                       std::map<char, std::string>& codes) {
    if (!node) return;
    if (!node->left && !node->right) codes[node->symbol] = prefix.empty() ? "0" : prefix;
    buildCodes(node->left, prefix + "0", codes);
    buildCodes(node->right, prefix + "1", codes);
}

inline std::map<char, std::string> huffmanCodes(const std::string& input) {
    struct Compare { bool operator()(HuffmanNode* a, HuffmanNode* b) const {
        return a->frequency > b->frequency;
    }};
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> q;
    int frequency[256] = {};
    for (unsigned char c : input) ++frequency[c];
    for (int i = 0; i < 256; ++i) if (frequency[i]) q.push(new HuffmanNode(static_cast<char>(i), frequency[i]));
    while (q.size() > 1) {
        auto* a = q.top(); q.pop(); auto* b = q.top(); q.pop();
        auto* parent = new HuffmanNode('\0', a->frequency + b->frequency);
        parent->left = a; parent->right = b; q.push(parent);
    }
    std::map<char, std::string> codes;
    if (!q.empty()) buildCodes(q.top(), "", codes);
    // The tree is intentionally short-lived; release every allocated node.
    std::function<void(HuffmanNode*)> release = [&](HuffmanNode* node) {
        if (!node) return; release(node->left); release(node->right); delete node;
    };
    if (!q.empty()) release(q.top());
    return codes;
}

inline std::string encodeHuffman(const std::string& input,
                                 const std::map<char, std::string>& codes) {
    std::string encoded;
    for (char c : input) {
        auto it = codes.find(c);
        if (it != codes.end()) encoded += it->second;
    }
    return encoded;
}

} // namespace advanced
#endif
