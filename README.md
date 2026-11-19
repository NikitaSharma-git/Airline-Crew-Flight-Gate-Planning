# Airline Crew, Flight and Gate Planning Suite

A C++17 student project that models airline operations using flight, crew, gate and airport-route data.

## Project status

**Integrated DAA suite – Phases 1–8 implemented; local validation and final refinements in progress**

The application currently provides:
- CSV loading for sample flights, crew and gates.
- Menu-driven command-line interface.
- Flight listing, linear flight-ID lookup, crew and gate directories.
- Basic overlapping gate-assignment conflict detection.
- Merge Sort and Quick Sort for flight IDs.
- Iterative and Recursive Binary Search for flight IDs.
- Comparison counters for sorting and searching.
- Maximum Subarray analysis on actual flight delay values.
- Celebrity Problem analysis on the directed airport route network.
- Strassen multiplication of the airport connectivity matrix to count two-leg routes.
- Activity Selection to choose the maximum number of non-overlapping flights assigned to a selected gate.
- Interval Partitioning to calculate the minimum number of temporary gates required for the loaded flight schedule.
- Dijkstra shortest-path analysis using scheduled flight durations between airports.


## Phase 4 greedy algorithms

| Algorithm | How it is used in this project | Time complexity |
|---|---|---|
| Activity Selection | For a selected gate, sorts its assigned flights by arrival time and chooses the maximum-size set of non-overlapping flights by repeatedly selecting the earliest-finishing compatible flight. | O(n²) with manual insertion sort; selection scan is O(n) |
| Interval Partitioning | Sorts all flights by departure time and reuses a temporary gate whenever its previous flight has finished. The number of temporary gates created is the minimum required for the schedule. | O(n²) with manual insertion sort and linear gate search |

Menu option 13 asks for a gate ID and displays the selected compatible flights. Menu option 14 displays a temporary gate plan for all flights. The temporary plan does not overwrite the original gate assignments in the CSV file.

| Prim Minimum Spanning Tree | Builds a minimum-weight undirected airport connection network by repeatedly adding the cheapest edge that expands the current tree. | O(V²) with an adjacency matrix |\n| Kruskal Minimum Spanning Tree | Builds a minimum-weight airport connection network by sorting route edges and joining components without creating cycles. | O(E log E) for edge sorting; DSU operations are near-constant amortized |\n\nMenu option 16 runs Prim and option 17 runs Kruskal. Both treat scheduled flight durations as undirected connection-cost estimates. If the route network is disconnected, they report a minimum spanning forest and its component count.\n\n| Dijkstra Shortest Path | Finds the minimum total scheduled flight duration between two selected airports using the directed flight network. | O(V²) with an adjacency matrix |

Menu option 15 asks the user to select source and destination airports. The route weights are flight durations; connection and waiting times are not included.

## Phase 3 algorithms

| Algorithm | How it is used in this project | Time complexity |
|---|---|---|
| Maximum Subarray | Finds the consecutive departure-time-ordered flights with the highest combined net delay impact. Positive delay means late; negative means early. | O(n log n) |
| Celebrity Problem | Finds a destination-only airport: every other airport has a direct route to it, and it has no outgoing route to other airports. | O(V) |
| Strassen Matrix Multiplication | Squares the airport adjacency matrix. Each result cell counts two-leg paths through one intermediate airport. | O(V^log₂7), approximately O(V²·⁸⁰⁷) |

For Strassen, matrix dimensions are padded to the next power of two and the result is cropped back to the airport count. All three algorithms use the flights loaded from `data/flights.csv`; they do not ask the user to enter unrelated sample arrays or matrices.

## Phase 2 algorithm notes

| Algorithm | Purpose | Time complexity |
|---|---|---|
| Merge Sort | Sort flights by ID | O(n log n) in best, average and worst cases |
| Quick Sort | Sort flights by ID | O(n log n) average; O(n²) worst case |
| Iterative Binary Search | Find a flight in an ID-sorted list | O(log n) |
| Recursive Binary Search | Find a flight in an ID-sorted list | O(log n) |

**Important:** Binary Search requires the flight list to be sorted by flight ID first. Run option 6 or 7 before options 8 or 9. Option 2 remains Linear Search and works regardless of sorting order.

## Integrated advanced DAA lab (main-menu option 18)

The advanced lab is opened from the main menu and keeps the earlier options 1–17 intact.

| Topic | Algorithm | Airline-planning use |
|---|---|---|
| Greedy | Huffman Coding | Builds prefix codes for a flight-operation text manifest and reports encoded length. |
| Dynamic Programming | 0/1 Knapsack | Selects high-delay-priority flights under a limited available-crew budget; crew requirement is the item weight and delay priority is the value. |
| Dynamic Programming | Matrix Chain Multiplication | Finds the least-cost multiplication order for user-entered matrix dimensions. |
| Dynamic Programming | Longest Common Subsequence | Compares the ordered source and destination airport label sequences. |
| Dynamic Programming | Optimal BST | Uses incoming-flight counts as airport lookup-frequency weights. |
| Graph DP | Bellman-Ford | Calculates single-source airport route costs and detects reachable negative cycles. |
| Graph DP | Floyd-Warshall | Calculates all-pairs shortest scheduled flight-duration paths. |
| Backtracking / B&B | Bounded TSP | Searches for a minimum-cost directed tour through the loaded airports, bounded to 12 airports. |
| Backtracking | N-Queens | Demonstrates constraint-based backtracking with a board size from 1 to 12. |
| Backtracking | Subset Sum | Finds a subset of flight crew requirements matching a target crew total. |
| Branch & Bound | FIFO, LIFO and best-bound search | Solves the crew-budget flight-priority knapsack with three node-selection strategies. |
| Network Flow | Ford-Fulkerson (BFS augmenting paths) | Computes flow over the airport route graph using summed required-crew values as an explicitly illustrative capacity proxy. |
| Matching | Bipartite Matching | Matches available crew members to individual flight-duty slots when their availability covers the flight. |
| Number Theory | Chinese Remainder Theorem | Finds a synchronized time offset for user-entered pairwise-coprime schedule cycles. |
| String Matching | Naive, Rabin-Karp, Finite Automaton and KMP | Searches a generated flight manifest and displays match positions from all four methods. |
| Integration | CSV export | Writes the loaded flight manifest to data/flight_manifest_export.csv. |

### Modeling assumptions and limitations

- Flight-route edge weights are scheduled flight durations. These are not complete itinerary times because layovers, transfer buffers, aircraft rotations and time zones are not modeled.
- The network-flow example uses required-crew totals as a teaching capacity proxy, not passenger or aircraft capacity. It should not be interpreted as a real-world airline capacity forecast.
- Knapsack prioritizes flights by positive delay minutes plus one, with required crew as resource consumption. This is an academic priority model, not an operational dispatch decision.
- Optimal-BST weights are derived from the number of incoming sample flights, not measured search logs.
- The CRT tool accepts schedule congruences from the user and requires pairwise-coprime positive moduli.
- The bounded TSP search is exponential and is intentionally restricted to at most 12 airports.
- The finite-automaton string matcher builds a byte-alphabet transition table and is intended for educational-sized manifests.

## Automated tests and benchmarking

Build the application, test executable and benchmark runner:

    cmake -S . -B build -DBUILD_TESTING=ON
    cmake --build build --parallel

Run automated algorithm tests:

    ctest --test-dir build --output-on-failure

Run the string-matching comparison:

    ./build/algorithm_benchmark

On Windows, run build\algorithm_benchmark.exe.

The GitHub Actions workflow in .github/workflows/cpp.yml configures CMake, builds the targets and runs CTest on pushes and pull requests to main.

The tests cover representative known-answer cases for the dynamic-programming, backtracking, branch-and-bound, flow, matching, CRT, string-matching and Huffman implementations. Benchmark timings depend on the machine and compiler; they are measurements, not fixed performance claims.

## Requirements

- C++17 compiler (GCC/MinGW, Clang or MSVC)
- CMake 3.16 or newer (recommended)

## Build with CMake

From the repository root:

```bash
cmake -S . -B build
cmake --build build
```

Run the application from the repository root so the relative CSV data paths resolve.

## Alternative: compile directly with g++

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/main.cpp -o airline_planner
```

On Windows PowerShell:

```powershell
.\\airline_planner.exe
```

## Sample data

CSV files are stored in `data/`:
- `flights.csv` includes a `delay_minutes` column. Positive values indicate late arrivals; negative values indicate early arrivals.
- `crew.csv`
- `gates.csv`

Times are represented as minutes after midnight. For example, 480 represents 08:00.

## Planned modules

1. Project foundation and data management
2. Sorting and searching algorithms
3. Divide and Conquer algorithms
4. Greedy algorithms and graph algorithms
5. Dynamic Programming
6. Backtracking and Branch & Bound
7. Network flow, matching, string matching and CRT
8. Integration, benchmarking, testing and final documentation

## Academic note

Mandatory DAA algorithms are implemented directly rather than replaced with library sorting or searching calls. Standard library containers and utilities support the application.

## License

For academic and educational use.
