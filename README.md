# Airline Crew, Flight and Gate Planning Suite

A C++17 student project that models airline operations, including flights, crew members, airport gates, scheduling constraints, conflict detection and disruption recovery.

## Project status

**Phase 1 – Foundation**

The initial version provides:
- Core Flight, Crew and Gate data structures.
- CSV loading for sample flights, crew and gates.
- A menu-driven command-line interface.
- Flight listing and flight-ID lookup.
- Crew and gate directories.
- Basic overlapping gate-assignment conflict detection.
- A CMake build configuration.

More DAA algorithms and scheduling features will be integrated in subsequent phases.

## Requirements

- C++17 compiler (GCC/MinGW, Clang or MSVC)
- CMake 3.16 or newer (recommended)

## Build with CMake

From the repository root:

```bash
cmake -S . -B build
cmake --build build
```

Run the application from the repository root so the relative CSV data paths resolve:

**Windows**
```bash
build\Debug\airline_planner.exe
```

For a single-configuration generator such as MinGW Makefiles:

```bash
build/airline_planner.exe
```

**Linux/macOS**
```bash
./build/airline_planner
```

## Alternative: compile directly with g++

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/main.cpp -o airline_planner
```

Run from the repository root:

```bash
./airline_planner
```

On Windows, run `airline_planner.exe`.

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

Mandatory DAA algorithms will be implemented as student-owned implementations rather than replaced with library algorithm calls. Standard library containers and utilities may be used for supporting application code.

## License

For academic and educational use.
