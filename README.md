# Airline Crew, Flight and Gate Planning Suite

A C++17 student project that models airline operations, including flights, crew members, airport gates, scheduling constraints, conflict detection and disruption recovery.

## Project status

**Phase 2 – Sorting and Searching**

The application currently provides:
- Core Flight, Crew and Gate data structures.
- CSV loading for sample flights, crew and gates.
- A menu-driven command-line interface.
- Flight listing and linear flight-ID lookup.
- Crew and gate directories.
- Basic overlapping gate-assignment conflict detection.
- Merge Sort and Quick Sort implementations for sorting flights by flight ID.
- Iterative and Recursive Binary Search implementations for flight-ID lookup.
- Comparison counters for the sorting and binary-search operations.

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

On Windows PowerShell, run:

```powershell
.\airline_planner.exe
```

## Sample data

CSV files are stored in `data/`:
- `flights.csv`
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

Mandatory DAA algorithms are implemented directly rather than replaced with library sorting or searching calls. Standard library containers and utilities are used for supporting application code.

## License

For academic and educational use.
