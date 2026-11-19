#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "models.hpp"
#include "sorting_searching.hpp"

namespace {

std::vector<std::string> splitCsvLine(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream stream(line);
    std::string field;

    while (std::getline(stream, field, ',')) fields.push_back(field);
    return fields;
}

bool loadFlights(const std::string& path, std::vector<Flight>& flights) {
    std::ifstream file(path);
    if (!file) return false;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        const auto fields = splitCsvLine(line);
        if (fields.size() != 8) continue;

        try {
            Flight flight;
            flight.flightID = std::stoi(fields[0]);
            flight.airline = fields[1];
            flight.source = fields[2];
            flight.destination = fields[3];
            flight.departureTime = std::stoi(fields[4]);
            flight.arrivalTime = std::stoi(fields[5]);
            flight.requiredCrew = std::stoi(fields[6]);
            flight.assignedGate = std::stoi(fields[7]);
            flights.push_back(flight);
        } catch (const std::exception&) {
            std::cerr << "Skipping invalid flight record: " << line << '\n';
        }
    }
    return true;
}

bool loadCrew(const std::string& path, std::vector<Crew>& crewMembers) {
    std::ifstream file(path);
    if (!file) return false;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        const auto fields = splitCsvLine(line);
        if (fields.size() != 5) continue;

        try {
            Crew member;
            member.crewID = std::stoi(fields[0]);
            member.name = fields[1];
            member.availableFrom = std::stoi(fields[2]);
            member.availableUntil = std::stoi(fields[3]);
            member.assigned = (fields[4] == "true");
            crewMembers.push_back(member);
        } catch (const std::exception&) {
            std::cerr << "Skipping invalid crew record: " << line << '\n';
        }
    }
    return true;
}

bool loadGates(const std::string& path, std::vector<Gate>& gates) {
    std::ifstream file(path);
    if (!file) return false;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        const auto fields = splitCsvLine(line);
        if (fields.size() != 4) continue;

        try {
            Gate gate;
            gate.gateID = std::stoi(fields[0]);
            gate.terminal = fields[1];
            gate.capacity = std::stoi(fields[2]);
            gate.available = (fields[3] == "true");
            gates.push_back(gate);
        } catch (const std::exception&) {
            std::cerr << "Skipping invalid gate record: " << line << '\n';
        }
    }
    return true;
}

void showFlightDetails(const Flight& flight) {
    std::cout << "Flight " << flight.flightID << ": "
              << flight.airline << ", " << flight.source << " to "
              << flight.destination
              << ", departure " << formatTime(flight.departureTime)
              << ", arrival " << formatTime(flight.arrivalTime)
              << ", crew " << flight.requiredCrew
              << ", gate " << flight.assignedGate << '\n';
}

void showFlights(const std::vector<Flight>& flights) {
    std::cout << "\nFLIGHT SCHEDULE\n";
    std::cout << "--------------------------------------------------------------------------\n";
    std::cout << "ID     Airline      Route             Departure  Arrival  Crew  Gate\n";
    std::cout << "--------------------------------------------------------------------------\n";

    for (const auto& flight : flights) {
        std::cout << flight.flightID << "  "
                  << flight.airline << "  "
                  << flight.source << "-" << flight.destination << "  "
                  << formatTime(flight.departureTime) << "       "
                  << formatTime(flight.arrivalTime) << "     "
                  << flight.requiredCrew << "     "
                  << (flight.assignedGate == 0 ? "Unassigned" : std::to_string(flight.assignedGate))
                  << '\n';
    }
}

void showCrew(const std::vector<Crew>& crewMembers) {
    std::cout << "\nCREW DIRECTORY\n";
    std::cout << "-----------------------------------------------\n";
    for (const auto& member : crewMembers) {
        std::cout << "ID: " << member.crewID
                  << " | " << member.name
                  << " | Available: " << formatTime(member.availableFrom)
                  << "-" << formatTime(member.availableUntil)
                  << " | " << (member.assigned ? "Assigned" : "Available")
                  << '\n';
    }
}

void showGates(const std::vector<Gate>& gates) {
    std::cout << "\nGATE DIRECTORY\n";
    std::cout << "-----------------------------------------------\n";
    for (const auto& gate : gates) {
        std::cout << "Gate " << gate.gateID
                  << " | Terminal " << gate.terminal
                  << " | Capacity: " << gate.capacity
                  << " | " << (gate.available ? "Available" : "Unavailable")
                  << '\n';
    }
}

void detectGateConflicts(const std::vector<Flight>& flights) {
    bool foundConflict = false;
    std::cout << "\nGATE CONFLICT REPORT\n";

    for (std::size_t i = 0; i < flights.size(); ++i) {
        for (std::size_t j = i + 1; j < flights.size(); ++j) {
            const auto& first = flights[i];
            const auto& second = flights[j];

            if (first.assignedGate == 0 || first.assignedGate != second.assignedGate) continue;

            const bool overlaps =
                first.departureTime < second.arrivalTime &&
                second.departureTime < first.arrivalTime;

            if (overlaps) {
                foundConflict = true;
                std::cout << "Conflict: Flight " << first.flightID
                          << " and Flight " << second.flightID
                          << " both use Gate " << first.assignedGate << ".\n";
            }
        }
    }

    if (!foundConflict) std::cout << "No overlapping gate assignments found.\n";
}

void searchFlightLinear(const std::vector<Flight>& flights) {
    int targetID;
    std::cout << "Enter flight ID: ";
    if (!(std::cin >> targetID)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid flight ID.\n";
        return;
    }

    for (const auto& flight : flights) {
        if (flight.flightID == targetID) {
            showFlightDetails(flight);
            return;
        }
    }
    std::cout << "Flight not found.\n";
}

void sortFlightsMerge(std::vector<Flight>& flights) {
    unsigned long long comparisons = 0;
    if (!flights.empty()) {
        mergeSortFlights(flights, 0, static_cast<int>(flights.size()) - 1, comparisons);
    }
    std::cout << "Flights sorted by ID using Merge Sort. Comparisons: "
              << comparisons << '\n';
    showFlights(flights);
}

void sortFlightsQuick(std::vector<Flight>& flights) {
    unsigned long long comparisons = 0;
    if (!flights.empty()) {
        quickSortFlights(flights, 0, static_cast<int>(flights.size()) - 1, comparisons);
    }
    std::cout << "Flights sorted by ID using Quick Sort. Comparisons: "
              << comparisons << '\n';
    showFlights(flights);
}

void binarySearchFlight(const std::vector<Flight>& flights, bool recursive) {
    int targetID;
    std::cout << "Enter flight ID to search: ";
    if (!(std::cin >> targetID)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid flight ID.\n";
        return;
    }

    unsigned long long comparisons = 0;
    int index = -1;

    if (recursive) {
        index = recursiveBinarySearch(flights, targetID, 0,
                                      static_cast<int>(flights.size()) - 1, comparisons);
    } else {
        index = iterativeBinarySearch(flights, targetID, comparisons);
    }

    std::cout << (recursive ? "Recursive" : "Iterative")
              << " Binary Search comparisons: " << comparisons << '\n';

    if (index == -1) {
        std::cout << "Flight not found.\n";
    } else {
        showFlightDetails(flights[index]);
    }
}

void showMenu() {
    std::cout << "\n===============================================\n";
    std::cout << " AIRLINE CREW, FLIGHT AND GATE PLANNING SUITE\n";
    std::cout << "===============================================\n";
    std::cout << "1. View all flights\n";
    std::cout << "2. Search flight by ID (Linear Search)\n";
    std::cout << "3. View crew members\n";
    std::cout << "4. View airport gates\n";
    std::cout << "5. Detect gate conflicts\n";
    std::cout << "6. Sort flights using Merge Sort\n";
    std::cout << "7. Sort flights using Quick Sort\n";
    std::cout << "8. Search flight using Iterative Binary Search\n";
    std::cout << "9. Search flight using Recursive Binary Search\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose an option: ";
}

} // namespace

int main() {
    std::vector<Flight> flights;
    std::vector<Crew> crewMembers;
    std::vector<Gate> gates;

    if (!loadFlights("data/flights.csv", flights) ||
        !loadCrew("data/crew.csv", crewMembers) ||
        !loadGates("data/gates.csv", gates)) {
        std::cerr << "Could not load project data. Run the program from the "
                     "repository root directory.\n";
        return 1;
    }

    std::cout << "Loaded " << flights.size() << " flights, "
              << crewMembers.size() << " crew members and "
              << gates.size() << " gates.\n";

    int choice = -1;
    while (choice != 0) {
        showMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Please enter a valid menu number.\n";
            continue;
        }

        switch (choice) {
            case 1: showFlights(flights); break;
            case 2: searchFlightLinear(flights); break;
            case 3: showCrew(crewMembers); break;
            case 4: showGates(gates); break;
            case 5: detectGateConflicts(flights); break;
            case 6: sortFlightsMerge(flights); break;
            case 7: sortFlightsQuick(flights); break;
            case 8: binarySearchFlight(flights, false); break;
            case 9: binarySearchFlight(flights, true); break;
            case 0: std::cout << "Exiting Airline Planning Suite. Goodbye!\n"; break;
            default: std::cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}
