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

include(CTest)
if(BUILD_TESTING)
    add_executable(advanced_tests tests/test_advanced.cpp)
    target_include_directories(advanced_tests PRIVATE include)
    add_test(NAME advanced_algorithm_tests COMMAND advanced_tests)
endif()

add_executable(algorithm_benchmark benchmarks/benchmark.cpp)
target_include_directories(algorithm_benchmark PRIVATE include)
