cmake_minimum_required(VERSION 3.16)
project(AirlineCrewFlightGatePlanning VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(airline_planner
    src/main.cpp
)

target_include_directories(airline_planner PRIVATE include)

if(MSVC)
    target_compile_options(airline_planner PRIVATE /W4)
else()
    target_compile_options(airline_planner PRIVATE -Wall -Wextra -Wpedantic)
endif()
