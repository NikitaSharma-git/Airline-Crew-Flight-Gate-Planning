#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "models.hpp"
#include "sorting_searching.hpp"
#include "divide_conquer.hpp"
#include "greedy_algorithms.hpp"
#include "graph_algorithms.hpp"
#include "advanced_menu.hpp"

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
        if (fields.size() != 9) continue;

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
            flight.delayMinutes = std::stoi(fields[8]);
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
    std::cout << "10. Analyze consecutive flight delays (Maximum Subarray)\n";
    std::cout << "11. Find destination-only airport (Celebrity Problem)\n";
    std::cout << "12. Analyze two-leg airport connections (Strassen)\n";
    std::cout << "13. Select maximum non-overlapping flights for a gate (Activity Selection)\n";
    std::cout << "14. Calculate minimum gates required (Interval Partitioning)\n";
    std::cout << "15. Find shortest airport route (Dijkstra)\n";
    std::cout << "16. Build minimum airport network (Prim)\\n";
    std::cout << "17. Build minimum airport network (Kruskal)\\n";
    std::cout << "18. Open Advanced DAA Algorithm Lab\\n";
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
            case 10: {
                if (flights.empty()) {
                    std::cout << "No flight records are available.\n";
                    break;
                }

                std::vector<Flight> chronological = flights;
                // Insertion sort by departure time, keeping the original flight records.
                for (std::size_t i = 1; i < chronological.size(); ++i) {
                    Flight current = chronological[i];
                    int j = static_cast<int>(i) - 1;
                    while (j >= 0 && chronological[j].departureTime > current.departureTime) {
                        chronological[j + 1] = chronological[j];
                        --j;
                    }
                    chronological[j + 1] = current;
                }

                std::vector<long long> delays;
                for (const auto& flight : chronological) delays.push_back(flight.delayMinutes);
                MaxSubarrayResult result = maximumSubarray(delays, 0, static_cast<int>(delays.size()) - 1);

                std::cout << "\nMAXIMUM CONSECUTIVE FLIGHT DELAY IMPACT\n";
                std::cout << "Flights in the highest-impact consecutive period: ";
                for (int i = result.left; i <= result.right; ++i) {
                    if (i > result.left) std::cout << ", ";
                    std::cout << chronological[i].flightID << " (" << chronological[i].delayMinutes << " min)";
                }
                std::cout << "\nCombined net delay impact: " << result.sum << " minutes\n";
                std::cout << "Algorithm: Divide and Conquer Maximum Subarray, O(n log n).\n";
                break;
            }
            case 11: {
                std::vector<std::string> airports;
                for (const auto& flight : flights) {
                    bool sourceFound = false, destinationFound = false;
                    for (const auto& airport : airports) {
                        if (airport == flight.source) sourceFound = true;
                        if (airport == flight.destination) destinationFound = true;
                    }
                    if (!sourceFound) airports.push_back(flight.source);
                    if (!destinationFound) airports.push_back(flight.destination);
                }

                const int n = static_cast<int>(airports.size());
                std::vector<std::vector<int>> routes(n, std::vector<int>(n, 0));
                for (const auto& flight : flights) {
                    int from = -1, to = -1;
                    for (int i = 0; i < n; ++i) {
                        if (airports[i] == flight.source) from = i;
                        if (airports[i] == flight.destination) to = i;
                    }
                    if (from != -1 && to != -1) routes[from][to] = 1;
                }

                int celebrity = findCelebrity(routes);
                std::cout << "\nDESTINATION-ONLY AIRPORT ANALYSIS\n";
                if (celebrity == -1) {
                    std::cout << "No destination-only airport exists in the current flight network.\n";
                } else {
                    std::cout << airports[celebrity]
                              << " is a destination-only airport: every other airport has a direct route to it, "
                                 "and it has no outgoing route to the others.\n";
                }
                std::cout << "Algorithm: Celebrity candidate elimination, O(V).\n";
                break;
            }
            case 12: {
                std::vector<std::string> airports;
                for (const auto& flight : flights) {
                    bool sourceFound = false, destinationFound = false;
                    for (const auto& airport : airports) {
                        if (airport == flight.source) sourceFound = true;
                        if (airport == flight.destination) destinationFound = true;
                    }
                    if (!sourceFound) airports.push_back(flight.source);
                    if (!destinationFound) airports.push_back(flight.destination);
                }

                if (airports.empty()) {
                    std::cout << "No airport route data is available.\n";
                    break;
                }

                int n = static_cast<int>(airports.size());
                Matrix connections(n, std::vector<long long>(n, 0));
                for (const auto& flight : flights) {
                    int from = -1, to = -1;
                    for (int i = 0; i < n; ++i) {
                        if (airports[i] == flight.source) from = i;
                        if (airports[i] == flight.destination) to = i;
                    }
                    if (from != -1 && to != -1) connections[from][to] = 1;
                }

                Matrix twoLegRoutes = strassenMatrixMultiply(connections, connections);
                std::cout << "\nTWO-LEG AIRPORT CONNECTIONS\n";
                std::cout << "Each cell shows the number of two-flight paths from its row airport to its column airport.\n";
                printMatrix(twoLegRoutes, airports);
                std::cout << "Algorithm: Strassen Matrix Multiplication, O(V^log2(7)) approximately O(V^2.807).\n";
                break;
            }
            case 13: {
                int gateID;
                std::cout << "Enter gate ID: ";
                if (!(std::cin >> gateID)) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cout << "Invalid gate ID.\n";
                    break;
                }

                std::vector<Flight> gateFlights;
                for (const auto& flight : flights) {
                    if (flight.assignedGate == gateID) gateFlights.push_back(flight);
                }

                if (gateFlights.empty()) {
                    std::cout << "No flights are currently assigned to Gate " << gateID << ".\n";
                    break;
                }

                FlightSelectionResult result = selectMaximumNonOverlappingFlights(gateFlights);
                std::cout << "\nACTIVITY SELECTION - GATE " << gateID << "\n";
                std::cout << "Maximum compatible flights: " << result.selectedFlights.size() << "\n";
                for (const auto& flight : result.selectedFlights) {
                    std::cout << "Flight " << flight.flightID << " | "
                              << formatTime(flight.departureTime) << "-"
                              << formatTime(flight.arrivalTime) << "\n";
                }
                std::cout << "Greedy rule: repeatedly choose the compatible flight that finishes earliest.\n";
                std::cout << "Time complexity: O(n^2) with the manual insertion sort used here.\n";
                break;
            }
            case 14: {
                if (flights.empty()) {
                    std::cout << "No flights are available for gate planning.\n";
                    break;
                }

                IntervalPartitionResult result = allocateMinimumGates(flights);
                std::cout << "\nINTERVAL PARTITIONING - TEMPORARY GATE PLAN\n";
                std::cout << "Minimum gates required: " << result.minimumGates << "\n";
                for (const auto& allocation : result.allocations) {
                    std::cout << "Flight " << allocation.flight.flightID << " | "
                              << formatTime(allocation.flight.departureTime) << "-"
                              << formatTime(allocation.flight.arrivalTime)
                              << " -> Temporary Gate " << allocation.temporaryGate << "\n";
                }
                std::cout << "The plan reuses a gate when its previous flight has finished.\n";
                std::cout << "Time complexity: O(n^2) for sorting and gate selection.\n";
                std::cout << "Note: This is a planning result; it does not overwrite the CSV gate assignments.\n";
                break;
            }
            case 15: {
                std::vector<std::string> airports;
                for (const auto& flight : flights) {
                    bool sourceFound = false, destinationFound = false;
                    for (const auto& airport : airports) {
                        if (airport == flight.source) sourceFound = true;
                        if (airport == flight.destination) destinationFound = true;
                    }
                    if (!sourceFound) airports.push_back(flight.source);
                    if (!destinationFound) airports.push_back(flight.destination);
                }

                if (airports.empty()) {
                    std::cout << "No airport route data is available.\n";
                    break;
                }

                const int n = static_cast<int>(airports.size());
                const int infinity = std::numeric_limits<int>::max() / 4;
                std::vector<std::vector<int>> graph(n, std::vector<int>(n, 0));
                for (const auto& flight : flights) {
                    int from = -1, to = -1;
                    for (int i = 0; i < n; ++i) {
                        if (airports[i] == flight.source) from = i;
                        if (airports[i] == flight.destination) to = i;
                    }
                    int duration = flight.arrivalTime - flight.departureTime;
                    if (from != -1 && to != -1 && duration > 0 &&
                        (graph[from][to] == 0 || duration < graph[from][to])) {
                        graph[from][to] = duration;
                    }
                }

                std::cout << "\nAIRPORTS IN THE ROUTE NETWORK\n";
                for (int i = 0; i < n; ++i) std::cout << i + 1 << ". " << airports[i] << "\n";

                int sourceChoice, destinationChoice;
                std::cout << "Enter source airport number: ";
                if (!(std::cin >> sourceChoice) || sourceChoice < 1 || sourceChoice > n) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cout << "Invalid source airport number.\n";
                    break;
                }
                std::cout << "Enter destination airport number: ";
                if (!(std::cin >> destinationChoice) || destinationChoice < 1 || destinationChoice > n) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cout << "Invalid destination airport number.\n";
                    break;
                }

                int source = sourceChoice - 1;
                int destination = destinationChoice - 1;
                DijkstraResult result = dijkstraShortestPath(graph, source);
                std::cout << "\nDIJKSTRA SHORTEST ROUTE\n";
                if (result.distance[destination] >= infinity) {
                    std::cout << "No directed route exists from " << airports[source]
                              << " to " << airports[destination] << ".\n";
                } else {
                    std::vector<int> path;
                    for (int vertex = destination; vertex != -1; vertex = result.parent[vertex]) {
                        path.push_back(vertex);
                    }
                    std::cout << "Shortest route: ";
                    for (int i = static_cast<int>(path.size()) - 1; i >= 0; --i) {
                        std::cout << airports[path[i]];
                        if (i > 0) std::cout << " -> ";
                    }
                    std::cout << "\nTotal flight duration: " << result.distance[destination]
                              << " minutes\n";
                }
                std::cout << "Algorithm: Dijkstra with an adjacency matrix, O(V^2).\n";
                std::cout << "Note: Route weights use scheduled flight durations, not connection/waiting times.\n";
                break;
            }
            case 16:
            case 17: {
                std::vector<std::string> airports;
                for (const auto& flight : flights) {
                    bool sourceFound = false, destinationFound = false;
                    for (const auto& airport : airports) {
                        if (airport == flight.source) sourceFound = true;
                        if (airport == flight.destination) destinationFound = true;
                    }
                    if (!sourceFound) airports.push_back(flight.source);
                    if (!destinationFound) airports.push_back(flight.destination);
                }

                if (airports.empty()) {
                    std::cout << "No airport route data is available.\\n";
                    break;
                }

                std::vector<WeightedEdge> edges;
                for (const auto& flight : flights) {
                    int from = -1, to = -1;
                    for (int i = 0; i < static_cast<int>(airports.size()); ++i) {
                        if (airports[i] == flight.source) from = i;
                        if (airports[i] == flight.destination) to = i;
                    }
                    int duration = flight.arrivalTime - flight.departureTime;
                    if (from == -1 || to == -1 || from == to || duration <= 0) continue;

                    int first = from < to ? from : to;
                    int second = from < to ? to : from;
                    bool updated = false;
                    for (auto& edge : edges) {
                        if (edge.from == first && edge.to == second) {
                            if (duration < edge.weight) edge.weight = duration;
                            updated = true;
                            break;
                        }
                    }
                    if (!updated) edges.push_back({first, second, duration});
                }

                MSTResult result = (choice == 16)
                    ? primMinimumSpanningForest(static_cast<int>(airports.size()), edges)
                    : kruskalMinimumSpanningForest(static_cast<int>(airports.size()), edges);

                std::cout << (choice == 16
                    ? "\\nPRIM MINIMUM SPANNING NETWORK\\n"
                    : "\\nKRUSKAL MINIMUM SPANNING NETWORK\\n");

                if (result.edges.empty() && airports.size() > 1) {
                    std::cout << "No airport connections are available to build a network.\\n";
                } else {
                    for (const auto& edge : result.edges) {
                        std::cout << airports[edge.from] << " -- " << airports[edge.to]
                                  << " | " << edge.weight << " minutes\\n";
                    }
                    std::cout << "Total network weight: " << result.totalWeight << " minutes\\n";
                    if (result.components == 1) {
                        std::cout << "All airports are connected in one minimum spanning network.\\n";
                    } else {
                        std::cout << "The route data is disconnected; displayed a minimum spanning forest with "
                                  << result.components << " components.\\n";
                    }
                }

                std::cout << "Algorithm: " << (choice == 16 ? "Prim" : "Kruskal")
                          << (choice == 16 ? ", O(V^2) with an adjacency matrix.\\n"
                                           : ", O(E log E) for sorting edges.\\n");
                std::cout << "Note: Scheduled flight durations are used as undirected connection-cost estimates.\\n";
                break;
            }
            case 18: runAdvancedAlgorithms(flights, crewMembers, gates); break;
            case 0: std::cout << "Exiting Airline Planning Suite. Goodbye!\n"; break;
            default: std::cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}
