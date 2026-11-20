# Airline Crew, Flight and Gate Planning

A compact C++17 DAA project demonstrating five algorithms through a simple airline-planning menu.

## Algorithms used
1. **Merge Sort** – sorts flights by departure time. Time: O(n log n).
2. **Binary Search** – finds a flight using its ID. Time: O(log n).
3. **Activity Selection** – selects the maximum number of non-overlapping flight activities. Time: O(n log n).
4. **Interval Partitioning** – calculates the minimum number of gates needed for the schedule. Time: O(n log n).
5. **Dijkstra's Algorithm** – finds shortest routes in a sample airport graph. With a priority queue: O((V + E) log V).

## Project features
- Menu-driven C++ command-line application.
- Sample flight records with flight IDs, airports, departure and arrival times.
- Demonstrations of all five algorithms using airline-related examples.

## Build and run

Using g++:
```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic src/main.cpp -o airline_planner
./airline_planner
```

Or use CMake:
```bash
cmake -S . -B build
cmake --build build
```

On Windows PowerShell, run `.airline_planner.exe` after compiling with MinGW.

## Notes
The flight records and airport graph are fixed sample data for educational demonstration. Route weights are illustrative units, not live distances or airline operating data. This project is not a real-world airline scheduling system.
