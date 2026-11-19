#ifndef AIRLINE_GREEDY_HPP
#define AIRLINE_GREEDY_HPP

#include <vector>
#include <algorithm>

#include "models.hpp"

struct FlightSelectionResult {
    std::vector<Flight> selectedFlights;
};

inline FlightSelectionResult selectMaximumNonOverlappingFlights(std::vector<Flight> flights) {
    // Sort by arrival time using insertion sort (earliest finishing flight first).
    for (std::size_t i = 1; i < flights.size(); ++i) {
        Flight current = flights[i];
        int j = static_cast<int>(i) - 1;
        while (j >= 0 && flights[j].arrivalTime > current.arrivalTime) {
            flights[j + 1] = flights[j];
            --j;
        }
        flights[j + 1] = current;
    }

    FlightSelectionResult result;
    int lastFinish = -1;
    for (const auto& flight : flights) {
        if (result.selectedFlights.empty() || flight.departureTime >= lastFinish) {
            result.selectedFlights.push_back(flight);
            lastFinish = flight.arrivalTime;
        }
    }
    return result;
}

struct GateAllocation {
    Flight flight;
    int temporaryGate;
};

struct IntervalPartitionResult {
    int minimumGates;
    std::vector<GateAllocation> allocations;
};

inline IntervalPartitionResult allocateMinimumGates(std::vector<Flight> flights) {
    // Sort flights by departure time using insertion sort.
    for (std::size_t i = 1; i < flights.size(); ++i) {
        Flight current = flights[i];
        int j = static_cast<int>(i) - 1;
        while (j >= 0 && flights[j].departureTime > current.departureTime) {
            flights[j + 1] = flights[j];
            --j;
        }
        flights[j + 1] = current;
    }

    std::vector<int> gateAvailableAt;
    IntervalPartitionResult result{0, {}};

    for (const auto& flight : flights) {
        int chosenGate = -1;

        // Reuse the first gate whose previous flight has finished.
        for (std::size_t gate = 0; gate < gateAvailableAt.size(); ++gate) {
            if (gateAvailableAt[gate] <= flight.departureTime) {
                chosenGate = static_cast<int>(gate);
                break;
            }
        }

        if (chosenGate == -1) {
            chosenGate = static_cast<int>(gateAvailableAt.size());
            gateAvailableAt.push_back(flight.arrivalTime);
        } else {
            gateAvailableAt[chosenGate] = flight.arrivalTime;
        }

        result.allocations.push_back({flight, chosenGate + 1});
    }

    result.minimumGates = static_cast<int>(gateAvailableAt.size());
    return result;
}

#endif
