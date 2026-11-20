cmake_minimum_required(VERSION 3.10)
project(AirlineCrewFlightGatePlanning)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_executable(airline_planner
    src/main.cpp src/flight.cpp src/crew.cpp src/gate.cpp
    src/airport.cpp src/algorithms.cpp src/reports.cpp src/storage.cpp)
target_include_directories(airline_planner PRIVATE include)
