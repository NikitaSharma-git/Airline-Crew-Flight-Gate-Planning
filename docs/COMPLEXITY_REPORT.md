# DAA Complexity Report

All bounds below describe the implemented algorithm, not CSV loading or console formatting.

| Algorithm | Time complexity | Space complexity |
|---|---:|---:|
| Merge Sort | O(n log n) | O(n) |
| Quick Sort | Average O(n log n), worst O(n²) | O(log n) average recursion |
| Binary Search (iterative / recursive) | O(log n) | O(1) / O(log n) |
| Maximum Subarray (divide and conquer) | O(n log n) | O(log n) recursion |
| Celebrity candidate elimination | O(V) | O(1) auxiliary |
| Strassen matrix multiplication | O(n^log₂7) | O(n²) plus recursion |
| Activity Selection | O(n²) including manual insertion sort | O(n) |
| Interval Partitioning | O(n²) with insertion sort and linear gate lookup | O(n) |
| Dijkstra (matrix) | O(V²) | O(V²) |
| Prim (matrix) | O(V²) | O(V²) |
| Kruskal (manual insertion sort) | O(E²) sorting plus near-constant DSU amortized | O(V + E) |
| Huffman coding | O(k log k) with a priority queue | O(k) |
| 0/1 Knapsack DP | O(nC) | O(nC) |
| Matrix Chain Multiplication | O(n³) | O(n²) |
| Longest Common Subsequence | O(mn) | O(mn) |
| Optimal BST | O(n³) | O(n²) |
| Bellman-Ford | O(VE) | O(V) |
| Floyd-Warshall | O(V³) | O(V²) |
| Bounded TSP | O(V!) worst case | O(V) recursion/path plus graph |
| N-Queens | O(N!) upper bound | O(N) |
| Subset Sum backtracking | O(2ⁿ) worst case | O(n) recursion |
| Branch-and-Bound Knapsack | O(2ⁿ) worst case | O(2ⁿ) pending nodes worst case |
| Ford-Fulkerson with BFS augmenting paths | O(VE²) | O(V²) residual matrix |
| Bipartite matching (augmenting paths) | O(VE) | O(V) |
| Chinese Remainder Theorem (extended GCD) | O(k log M) arithmetic operations | O(1) |
| Naive String Matching | O(nm) | O(1) excluding matches |
| Rabin-Karp | Expected O(n + m), worst O(nm) | O(1) excluding matches |
| Finite Automaton Matching | O(Σm + n) for fixed byte alphabet after transition construction | O(256m) |
| KMP | O(n + m) | O(m) |

Here n is the number of records/items, V airports/vertices, E route edges, C crew-budget capacity, k distinct symbols/congruences, and m/n denote pattern/text lengths where relevant. The project deliberately uses manual insertion sorting in the greedy/MST modules to keep the sorting implementation visible for DAA evaluation.

Some application models are educational proxies rather than operational forecasts: flight delay priority for knapsack, required-crew values as flow capacities, incoming-flight counts as optimal-BST frequencies, and flight durations as route costs.