#ifndef AIRLINE_ADVANCED_MENU_HPP
#define AIRLINE_ADVANCED_MENU_HPP

#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <regex>
#include <string>
#include <vector>

#include "advanced_algorithms.hpp"


inline bool readJsonStringField(const std::string& object, const std::string& key, std::string& value) {
    std::regex pattern("\"" + key + "\"\\s*:\\s*\"([^\"]*)\"");
    std::smatch match;
    if (!std::regex_search(object, match, pattern)) return false;
    value = match[1].str();
    return true;
}

inline bool readJsonIntField(const std::string& object, const std::string& key, int& value) {
    std::regex pattern("\"" + key + "\"\\s*:\\s*(-?[0-9]+)");
    std::smatch match;
    if (!std::regex_search(object, match, pattern)) return false;
    try { value = std::stoi(match[1].str()); return true; }
    catch (const std::exception&) { return false; }
}

inline std::string escapeJsonString(const std::string& value) {
    std::string escaped;
    for (char c : value) {
        if (c == '\\' || c == '"') escaped += '\\';
        if (c == '\n') { escaped += "\\n"; continue; }
        if (c == '\r') { escaped += "\\r"; continue; }
        escaped += c;
    }
    return escaped;
}

inline bool importFlightsJson(const std::string& path, std::vector<Flight>& flights, bool append) {
    std::ifstream input(path);
    if (!input) return false;
    std::string content((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    std::regex objectPattern("\\{[^\\}]*\\}");
    std::vector<Flight> imported;
    for (std::sregex_iterator it(content.begin(), content.end(), objectPattern), end; it != end; ++it) {
        const std::string object = it->str();
        Flight flight;
        if (!readJsonIntField(object, "flight_id", flight.flightID) ||
            !readJsonStringField(object, "airline", flight.airline) ||
            !readJsonStringField(object, "source", flight.source) ||
            !readJsonStringField(object, "destination", flight.destination) ||
            !readJsonIntField(object, "departure", flight.departureTime) ||
            !readJsonIntField(object, "arrival", flight.arrivalTime) ||
            !readJsonIntField(object, "required_crew", flight.requiredCrew) ||
            !readJsonIntField(object, "assigned_gate", flight.assignedGate) ||
            !readJsonIntField(object, "delay_minutes", flight.delayMinutes)) {
            return false;
        }
        imported.push_back(flight);
    }
    if (imported.empty()) return false;
    if (!append) flights.clear();
    for (const auto& flight : imported) flights.push_back(flight);
    return true;
}

inline bool exportFlightsJson(const std::string& path, const std::vector<Flight>& flights) {
    std::ofstream out(path);
    if (!out) return false;
    out << "{\n  \"flights\": [\n";
    for (std::size_t i = 0; i < flights.size(); ++i) {
        const auto& f = flights[i];
        out << "    {\"flight_id\": " << f.flightID
            << ", \"airline\": \"" << escapeJsonString(f.airline)
            << "\", \"source\": \"" << escapeJsonString(f.source)
            << "\", \"destination\": \"" << escapeJsonString(f.destination)
            << "\", \"departure\": " << f.departureTime
            << ", \"arrival\": " << f.arrivalTime
            << ", \"required_crew\": " << f.requiredCrew
            << ", \"assigned_gate\": " << f.assignedGate
            << ", \"delay_minutes\": " << f.delayMinutes << "}";
        if (i + 1 < flights.size()) out << ",";
        out << "\n";
    }
    out << "  ]\n}\n";
    return true;
}

inline void runAdvancedAlgorithms(std::vector<Flight>& flights,
                                 const std::vector<Crew>& crew,
                                 const std::vector<Gate>& gates) {
    using namespace advanced;
    std::cout << "Current data - flights: " << flights.size()
              << " | crew: " << crew.size() << " | gates: " << gates.size() << "\n";
    std::vector<std::string> airports;
    for (const auto& f : flights) {
        bool a = false, b = false;
        for (const auto& name : airports) {
            if (name == f.source) a = true;
            if (name == f.destination) b = true;
        }
        if (!a) airports.push_back(f.source);
        if (!b) airports.push_back(f.destination);
    }
    const int n = static_cast<int>(airports.size());
    const int inf = std::numeric_limits<int>::max() / 4;
    std::vector<std::vector<int>> graph(n, std::vector<int>(n, inf));
    std::vector<Edge> edges;
    for (int i = 0; i < n; ++i) graph[i][i] = 0;
    for (const auto& f : flights) {
        int u = -1, v = -1;
        for (int i = 0; i < n; ++i) {
            if (airports[i] == f.source) u = i;
            if (airports[i] == f.destination) v = i;
        }
        int duration = f.arrivalTime - f.departureTime;
        if (u >= 0 && v >= 0 && duration > 0) {
            if (graph[u][v] == inf || duration < graph[u][v]) graph[u][v] = duration;
            edges.push_back({u, v, duration});
        }
    }

    int option = -1;
    while (option != 0) {
        std::cout << "\n========== ADVANCED DAA ALGORITHM LAB ==========\n"
                  << "1. Huffman coding for flight operation text\n"
                  << "2. 0/1 Knapsack - prioritize flights with limited crew\n"
                  << "3. Matrix Chain Multiplication\n"
                  << "4. Longest Common Subsequence of flight route lists\n"
                  << "5. Optimal BST for airport lookup frequencies\n"
                  << "6. Bellman-Ford shortest paths\n"
                  << "7. Floyd-Warshall all-pairs shortest paths\n"
                  << "8. Bounded TSP airport tour\n"
                  << "9. N-Queens backtracking\n"
                  << "10. Subset Sum using crew requirements\n"
                  << "11. Branch and Bound Knapsack (FIFO / LIFO / Best-bound)\n"
                  << "12. Ford-Fulkerson route crew-capacity flow model\n"
                  << "13. Bipartite matching: crew to flight duty slots\n"
                  << "14. Chinese Remainder Theorem schedule synchronization\n"
                  << "15. String matching: Naive / Rabin-Karp / FA / KMP\n"
                  << "16. Export current flight manifest to CSV\n"
                  << "17. Import flight manifest from JSON\n"
                  << "18. Export current flight manifest to JSON\n"
                  << "0. Return to main menu\n"
                  << "Select: ";
        if (!(std::cin >> option)) {
            std::cin.clear(); std::cin.ignore(10000, '\n');
            std::cout << "Enter a valid number.\n"; continue;
        }

        if (option == 1) {
            std::string text = "AIRLINE FLIGHT GATE CREW ";
            for (const auto& f : flights)
                text += f.airline + " " + f.source + " " + f.destination + " ";
            auto codes = huffmanCodes(text);
            std::cout << "\nHUFFMAN CODES\n";
            for (const auto& item : codes)
                std::cout << (item.first == ' ' ? "[space]" : std::string(1, item.first))
                          << " : " << item.second << "\n";
            std::cout << "Original characters: " << text.size()
                      << " | Encoded bits: " << encodeHuffman(text, codes).size() << "\n";
            std::cout << "Huffman coding builds a frequency-based prefix code; construction O(k log k).\n";
        } else if (option == 2 || option == 11) {
            std::vector<int> weights, values;
            for (const auto& f : flights) {
                weights.push_back(f.requiredCrew);
                values.push_back((f.delayMinutes > 0 ? f.delayMinutes : 0) + 1);
            }
            int capacity = 0;
            for (const auto& c : crew) if (!c.assigned) ++capacity;
            std::cout << "\nAvailable crew members: " << capacity
                      << " | Flight crew requirements are the item weights.\n";
            std::cout << "Enter crew capacity to plan with (0 uses all available crew): ";
            int entered;
            if (!(std::cin >> entered)) { std::cin.clear(); std::cin.ignore(10000, '\n'); break; }
            if (entered > 0) capacity = entered;
            if (option == 2) {
                auto result = knapsack01(weights, values, capacity);
                std::cout << "Maximum priority score: " << result.value << "\nSelected flights: ";
                for (int i : result.selected) std::cout << flights[i].flightID << " ";
                std::cout << "\nDP complexity: O(number_of_flights x crew_capacity).\n";
            } else {
                std::cout << "Branch strategy (1 FIFO, 2 LIFO, 3 Least-bound): ";
                int strategy; std::cin >> strategy;
                if (strategy < 1 || strategy > 3) strategy = 1;
                auto result = knapsackBranchBound(weights, values, capacity, strategy - 1);
                std::cout << "Best priority score: " << result.value << "\nSelected flights: ";
                for (int i : result.selected) std::cout << flights[i].flightID << " ";
                std::cout << "\nSearch nodes explored: " << result.explored << "\n";
            }
        } else if (option == 3) {
            int count;
            std::cout << "Number of matrices: "; std::cin >> count;
            if (count < 1 || count > 20) { std::cout << "Use 1 to 20 matrices.\n"; continue; }
            std::vector<int> dims(count + 1);
            std::cout << "Enter " << count + 1 << " dimensions: ";
            for (int& d : dims) std::cin >> d;
            bool valid = true; for (int d : dims) if (d <= 0) valid = false;
            if (!valid) std::cout << "Dimensions must be positive.\n";
            else std::cout << "Minimum scalar multiplications: " << matrixChainMinimumCost(dims) << "\n";
        } else if (option == 4) {
            std::string sources, destinations;
            for (const auto& f : flights) { sources += f.source + "|"; destinations += f.destination + "|"; }
            std::cout << "Common ordered airport labels in source/destination lists: "
                      << longestCommonSubsequence(sources, destinations) << " characters.\n";
            std::cout << "LCS dynamic programming complexity: O(mn).\n";
        } else if (option == 5) {
            std::vector<std::pair<std::string, int>> airportWeights;
            for (const auto& airport : airports) {
                int incoming = 0;
                for (const auto& f : flights) if (f.destination == airport) ++incoming;
                airportWeights.push_back({airport, incoming + 1});
            }
            // Optimal BST keys must be ordered; sort airport names with insertion sort.
            for (std::size_t i = 1; i < airportWeights.size(); ++i) {
                auto current = airportWeights[i];
                int j = static_cast<int>(i) - 1;
                while (j >= 0 && airportWeights[j].first > current.first) {
                    airportWeights[j + 1] = airportWeights[j];
                    --j;
                }
                airportWeights[j + 1] = current;
            }
            std::vector<int> frequency;
            std::cout << "\nAirport lookup frequency weights (incoming flight count + 1):\n";
            for (const auto& item : airportWeights) {
                frequency.push_back(item.second);
                std::cout << item.first << ": " << item.second << "\n";
            }
            std::cout << "Optimal BST weighted search cost: " << optimalBstCost(frequency) << "\n";
        } else if (option == 6) {
            if (n == 0) { std::cout << "No airports loaded.\n"; continue; }
            std::cout << "Source airport:\n";
            for (int i = 0; i < n; ++i) std::cout << i + 1 << ". " << airports[i] << "\n";
            int source; std::cin >> source; --source;
            if (source < 0 || source >= n) { std::cout << "Invalid source.\n"; continue; }
            auto result = bellmanFord(n, edges, source);
            if (result.hasNegativeCycle) std::cout << "A reachable negative-weight cycle exists.\n";
            for (int i = 0; i < n; ++i)
                std::cout << airports[i] << ": "
                          << (result.distance[i] >= inf ? "unreachable" : std::to_string(result.distance[i]) + " min") << "\n";
            std::cout << "Bellman-Ford complexity: O(VE).\n";
        } else if (option == 7) {
            auto result = floydWarshall(graph);
            std::cout << "\nALL-PAIRS SHORTEST FLIGHT DURATIONS\n";
            for (int i = 0; i < n; ++i) {
                std::cout << airports[i] << ": ";
                for (int j = 0; j < n; ++j)
                    std::cout << (result[i][j] >= inf ? "INF" : std::to_string(result[i][j])) << " ";
                std::cout << "\n";
            }
            std::cout << "Floyd-Warshall complexity: O(V^3).\n";
        } else if (option == 8) {
            if (n < 2 || n > 12) { std::cout << "TSP demo supports 2 to 12 airports.\n"; continue; }
            std::cout << "Start airport:\n";
            for (int i = 0; i < n; ++i) std::cout << i + 1 << ". " << airports[i] << "\n";
            int start; std::cin >> start; --start;
            if (start < 0 || start >= n) { std::cout << "Invalid airport.\n"; continue; }
            auto result = boundedTsp(graph, start);
            if (result.path.empty()) std::cout << "No complete directed tour exists in the loaded flight routes.\n";
            else {
                std::cout << "Tour: ";
                for (std::size_t i = 0; i < result.path.size(); ++i)
                    std::cout << airports[result.path[i]] << (i + 1 < result.path.size() ? " -> " : "");
                std::cout << "\nTotal duration: " << result.cost << " minutes\n";
            }
            std::cout << "Search nodes explored: " << result.explored << "\n";
        } else if (option == 9) {
            int size; std::cout << "Board size N (1 to 12): "; std::cin >> size;
            if (size < 1 || size > 12) { std::cout << "N must be 1 to 12.\n"; continue; }
            std::vector<int> placement;
            if (!nQueensSolve(size, placement)) std::cout << "No solution exists for this N.\n";
            else {
                for (int r = 0; r < size; ++r) {
                    for (int c = 0; c < size; ++c) std::cout << (placement[r] == c ? "Q " : ". ");
                    std::cout << "\n";
                }
            }
        } else if (option == 10) {
            std::vector<int> requirements;
            int total = 0;
            for (const auto& f : flights) { requirements.push_back(f.requiredCrew); total += f.requiredCrew; }
            int target; std::cout << "Target crew total (1 to " << total << "): "; std::cin >> target;
            std::vector<int> chosen;
            if (subsetSum(requirements, target, chosen)) {
                std::cout << "Matching flight set: ";
                for (int i : chosen) std::cout << flights[i].flightID << " ";
                std::cout << "\n";
            } else std::cout << "No subset of flight crew requirements matches that total.\n";
        } else if (option == 12) {
            if (n < 2) { std::cout << "At least two airports are required.\n"; continue; }
            std::vector<std::vector<int>> capacity(n, std::vector<int>(n, 0));
            for (const auto& f : flights) {
                int u = -1, v = -1;
                for (int i = 0; i < n; ++i) {
                    if (airports[i] == f.source) u = i;
                    if (airports[i] == f.destination) v = i;
                }
                if (u >= 0 && v >= 0) capacity[u][v] += f.requiredCrew;
            }
            std::cout << "Airport indices:\n";
            for (int i = 0; i < n; ++i) std::cout << i + 1 << ". " << airports[i] << "\n";
            int s, t; std::cout << "Source and sink numbers: "; std::cin >> s >> t; --s; --t;
            if (s < 0 || s >= n || t < 0 || t >= n || s == t) std::cout << "Invalid source/sink.\n";
            else std::cout << "Maximum flow (crew-slot capacity proxy): "
                           << fordFulkerson(capacity, s, t) << "\n";
            std::cout << "Ford-Fulkerson uses augmenting paths; Edmonds-Karp BFS bound O(VE^2).\n";
        } else if (option == 13) {
            std::vector<std::vector<int>> adjacency;
            std::vector<std::pair<int, int>> slots;
            for (int i = 0; i < static_cast<int>(flights.size()); ++i)
                for (int slot = 0; slot < flights[i].requiredCrew; ++slot) {
                    std::vector<int> eligible;
                    for (int c = 0; c < static_cast<int>(crew.size()); ++c)
                        if (!crew[c].assigned &&
                            crew[c].availableFrom <= flights[i].departureTime &&
                            crew[c].availableUntil >= flights[i].arrivalTime)
                            eligible.push_back(c);
                    adjacency.push_back(eligible);
                    slots.push_back({i, slot});
                }
            std::vector<int> matching;
            int count = maximumBipartiteMatching(adjacency, static_cast<int>(crew.size()), matching);
            std::cout << "Matched crew-duty slots: " << count << " / " << slots.size() << "\n";
            for (int c = 0; c < static_cast<int>(matching.size()); ++c)
                if (matching[c] >= 0) {
                    auto slot = slots[matching[c]];
                    std::cout << crew[c].name << " -> Flight " << flights[slot.first].flightID
                              << " (crew slot " << slot.second + 1 << ")\n";
                }
            std::cout << "Matching complexity: O(VE) for this augmenting-path implementation.\n";
        } else if (option == 14) {
            int count; std::cout << "Number of schedule cycles: "; std::cin >> count;
            if (count < 1 || count > 10) { std::cout << "Use 1 to 10 congruences.\n"; continue; }
            std::vector<int> residues(count), moduli(count);
            for (int i = 0; i < count; ++i) {
                std::cout << "Enter required remainder and cycle length for schedule " << i + 1 << ": ";
                std::cin >> residues[i] >> moduli[i];
            }
            long long answer = 0;
            if (chineseRemainder(residues, moduli, answer))
                std::cout << "First synchronized time offset: " << answer << " time units.\n";
            else std::cout << "No unique CRT solution: moduli must be pairwise coprime and positive.\n";
        } else if (option == 15) {
            std::string manifest, pattern;
            for (const auto& f : flights)
                manifest += std::to_string(f.flightID) + " " + f.airline + " " +
                            f.source + " " + f.destination + " ";
            std::cout << "Enter text to find in the flight manifest (no spaces for one token): ";
            std::cin >> pattern;
            auto naive = naiveSearch(manifest, pattern);
            auto rk = rabinKarpSearch(manifest, pattern);
            auto fa = finiteAutomatonSearch(manifest, pattern);
            auto kmp = kmpSearch(manifest, pattern);
            std::cout << "Match positions (0-based):\nNaive: ";
            for (int p : naive) std::cout << p << " ";
            std::cout << "\nRabin-Karp: "; for (int p : rk) std::cout << p << " ";
            std::cout << "\nFinite automaton: "; for (int p : fa) std::cout << p << " ";
            std::cout << "\nKMP: "; for (int p : kmp) std::cout << p << " ";
            std::cout << "\n";
        } else if (option == 16) {
            std::ofstream out("data/flight_manifest_export.csv");
            if (!out) std::cout << "Could not create export file in data/.\n";
            else {
                out << "flight_id,airline,source,destination,departure,arrival,required_crew,assigned_gate,delay_minutes\n";
                for (const auto& f : flights)
                    out << f.flightID << "," << f.airline << "," << f.source << ","
                        << f.destination << "," << f.departureTime << "," << f.arrivalTime
                        << "," << f.requiredCrew << "," << f.assignedGate << "," << f.delayMinutes << "\n";
                std::cout << "Exported " << flights.size() << " records to data/flight_manifest_export.csv\n";
            }
        } else if (option == 17) {
            int mode;
            std::cout << "Import mode (1 replace current flights, 2 append): ";
            std::cin >> mode;
            if (mode != 1 && mode != 2) { std::cout << "Invalid import mode.\n"; continue; }
            if (importFlightsJson("data/flights.json", flights, mode == 2))
                std::cout << "JSON import completed. Current flight count: " << flights.size() << "\n";
            else
                std::cout << "JSON import failed. Check data/flights.json and its required field names.\n";
        } else if (option == 18) {
            if (exportFlightsJson("data/flight_manifest_export.json", flights))
                std::cout << "Exported " << flights.size() << " records to data/flight_manifest_export.json\n";
            else
                std::cout << "Could not create JSON export in data/.\n";
        } else if (option == 0) {
            std::cout << "Returning to the main menu.\n";
        } else {
            std::cout << "Invalid advanced option.\n";
        }
    }
}

#endif
