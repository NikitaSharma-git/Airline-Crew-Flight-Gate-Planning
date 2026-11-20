# Airline Crew, Flight and Gate Planning System

A menu-driven C++17 project that combines airline operations management with five core Design and Analysis of Algorithms concepts.

## Five DAA algorithms

| Algorithm | Application | Complexity |
|---|---|---|
| Merge Sort | Sort flight schedules by departure or arrival | O(n log n) |
| Binary Search | Find a flight by its ID in sorted records | O(log n) |
| Activity Selection | Select a maximum set of non-overlapping flights | O(n log n) including sorting |
| Interval Partitioning | Calculate the minimum gates needed for a schedule | O(n log n) |
| Dijkstra | Find a shortest path between airports | O((V + E) log V) with a priority queue |

## System modules

- **Flight Management:** add, view, update, delete, search and sort flights; update flight status.
- **Crew Management:** maintain crew records, assign crew to flights and release assignments.
- **Gate Management:** add and activate gates, assign/release gates and detect overlapping gate use.
- **Airport and Route Management:** add/remove airports and maintain two-way weighted routes.
- **DAA Algorithm Centre:** run demonstrations of all five algorithms using the current records.
- **Reports and Analytics:** view flight status totals, crew availability, gate usage and route frequency.
- **CSV Storage:** load records at startup and save changes to the data folder.

## Project structure

    include/       Header files and shared data models
    src/           Main program and module implementations
    data/          CSV records loaded and saved by the program

## Build with g++ (from the repository root)

    g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src/main.cpp src/flight.cpp src/crew.cpp src/gate.cpp src/airport.cpp src/algorithms.cpp src/reports.cpp src/storage.cpp -o airline_planner

Run on Linux/macOS:

    ./airline_planner

Run on Windows PowerShell:

    .\airline_planner.exe

## Build with CMake

    cmake -S . -B build
    cmake --build build

Run the executable from the repository root so that the relative data/*.csv paths resolve correctly. The application writes updated records to the CSV files when you choose Save All Data or Save and Exit.

## Notes

- The included flights, crew, gates, airports and route distances are sample educational data.
- Route distances are illustrative and are not live airline data.
- Crew assignment prevents assigning an already-assigned crew member, but does not model real duty-time regulations or overlapping shift calendars.
- The project demonstrates algorithmic ideas; it is not intended for real airline operations.
