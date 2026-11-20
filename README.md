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

- **Flight Management:** add, view, update, delete, search and sort flights; reschedule flights with crew and gate conflict checks; filter by status.
- **Crew Management:** maintain crew records, assign crew to multiple non-overlapping flights, prevent overlapping assignments, view crew schedules and summarise roles.
- **Gate Management:** add and activate gates, automatically or manually assign gates, release assignments, find gates available for a selected time slot and detect conflicts.
- **Airport and Route Management:** add/remove airports and maintain two-way weighted routes.
- **DAA Algorithm Centre:** run demonstrations of all five algorithms using the current records.
- **Reports and Analytics:** view flight status totals, delay percentage, average duration, crew availability, gate utilisation, route frequency and busiest airport.
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
- Crew scheduling checks overlapping flight intervals, but does not model real duty-time regulations, rest periods, qualifications or crew-team requirements.
- Flight and gate times are represented as minutes after midnight; the project uses a simplified schedule model.
- The project demonstrates algorithmic ideas; it is not intended for real airline operations.
