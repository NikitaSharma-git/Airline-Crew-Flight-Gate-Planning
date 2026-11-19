# Airline Crew, Flight and Gate Planning Suite

A C++17 student project that models airline operations using flight, crew, gate and airport-route data.

## Project status

**Phase 3 – Airline-focused Divide and Conquer**

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
