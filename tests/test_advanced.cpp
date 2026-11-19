#include <cassert>
#include <climits>
#include <iostream>
#include <vector>
#include "advanced_algorithms.hpp"

int main() {
    using namespace advanced;
    auto k = knapsack01({2, 3, 4}, {4, 5, 7}, 5);
    assert(k.value == 9);
    assert(matrixChainMinimumCost({10, 20, 30}) == 6000);
    assert(longestCommonSubsequence("ABCBDAB", "BDCABA") == 4);
    assert(optimalBstCost({34, 8, 50}) > 0);

    auto bf = bellmanFord(3, {{0, 1, 4}, {1, 2, 3}}, 0);
    assert(!bf.hasNegativeCycle && bf.distance[2] == 7);
    const int inf = INT_MAX / 4;
    auto fw = floydWarshall({{0, 4, inf}, {inf, 0, 3}, {inf, inf, 0}});
    assert(fw[0][2] == 7);

    auto tsp = boundedTsp({{0, 1, 5}, {5, 0, 2}, {3, 5, 0}}, 0);
    assert(tsp.cost == 6);
    std::vector<int> queens;
    assert(nQueensSolve(4, queens));
    std::vector<int> subset;
    assert(subsetSum({2, 3, 7}, 5, subset));

    auto bnb = knapsackBranchBound({2, 3, 4}, {4, 5, 7}, 5, 2);
    assert(bnb.value == 9);

    std::vector<std::vector<int>> capacity = {
        {0, 16, 13, 0, 0, 0}, {0, 0, 10, 12, 0, 0},
        {0, 4, 0, 0, 14, 0}, {0, 0, 9, 0, 0, 20},
        {0, 0, 0, 7, 0, 4}, {0, 0, 0, 0, 0, 0}
    };
    assert(fordFulkerson(capacity, 0, 5) == 23);

    std::vector<int> matching;
    assert(maximumBipartiteMatching({{0, 1}, {0}}, 2, matching) == 2);
    long long crt = 0;
    assert(chineseRemainder({2, 3}, {3, 5}, crt) && crt == 8);

    auto naive = naiveSearch("ABABDABACDABABCABAB", "ABABCABAB");
    auto rk = rabinKarpSearch("ABABDABACDABABCABAB", "ABABCABAB");
    auto kmp = kmpSearch("ABABDABACDABABCABAB", "ABABCABAB");
    auto fa = finiteAutomatonSearch("ABABDABACDABABCABAB", "ABABCABAB");
    assert(naive.size() == 1 && naive == rk && rk == kmp && kmp == fa);

    auto codes = huffmanCodes("AIRLINE AIRLINE");
    assert(!codes.empty());
    assert(!encodeHuffman("AIRLINE", codes).empty());

    std::cout << "All advanced algorithm tests passed.\n";
    return 0;
}
