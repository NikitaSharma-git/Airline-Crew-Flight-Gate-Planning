#include "reports.h"
#include <iostream>
#include <map>
#include <iomanip>
using namespace std;

void showReports(const vector<Flight>& flights,
                 const vector<Crew>& crew,
                 const vector<Gate>& gates) {
    int scheduled = 0, delayed = 0, cancelled = 0, assignedCrew = 0;
    long long totalDuration = 0;
    map<string, int> routeCounts, airportCounts;
    for (const auto& flight : flights) {
        if (flight.status == "Scheduled") ++scheduled;
        else if (flight.status == "Delayed") ++delayed;
        else if (flight.status == "Cancelled") ++cancelled;
        if (flight.crewId) ++assignedCrew;
        if (flight.status != "Cancelled") {
            totalDuration += flight.arrival - flight.departure;
            ++routeCounts[flight.from + "-" + flight.to];
            ++airportCounts[flight.from];
            ++airportCounts[flight.to];
        }
    }

    int availableCrew = 0, activeGates = 0, usedGates = 0;
    for (const auto& member : crew) if (member.available) ++availableCrew;
    for (const auto& gate : gates) {
        if (gate.active) ++activeGates;
        bool used = false;
        for (const auto& flight : flights)
            if (flight.gateId == gate.id && flight.status != "Cancelled") used = true;
        if (used) ++usedGates;
    }

    cout << "\n========== AIRLINE OPERATIONS REPORT ==========\n";
    cout << "Total flights            : " << flights.size() << '\n';
    cout << "Scheduled flights        : " << scheduled << '\n';
    cout << "Delayed flights          : " << delayed << '\n';
    cout << "Cancelled flights        : " << cancelled << '\n';
    cout << "Crew members             : " << crew.size() << '\n';
    cout << "Available crew           : " << availableCrew << '\n';
    cout << "Flights with crew assigned: " << assignedCrew << '\n';
    cout << "Active gates             : " << activeGates << '\n';
    cout << "Gates currently used     : " << usedGates << '\n';

    int activeFlights = static_cast<int>(flights.size()) - cancelled;
    cout << "Average flight duration  : ";
    if (activeFlights == 0) cout << "N/A\n";
    else cout << totalDuration / activeFlights << " minutes\n";
    cout << fixed << setprecision(1);
    cout << "Delay percentage         : ";
    if (activeFlights == 0) cout << "N/A\n";
    else cout << (100.0 * delayed / activeFlights) << "%\n";
    cout << "Gate utilisation         : ";
    if (activeGates == 0) cout << "N/A\n";
    else cout << (100.0 * usedGates / activeGates) << "%\n";

    cout << "\nRoute frequency:\n";
    if (routeCounts.empty()) cout << "No active flight data.\n";
    else {
        for (const auto& item : routeCounts)
            cout << item.first << " : " << item.second << " flight(s)\n";
        auto busiest = routeCounts.begin();
        for (auto it = routeCounts.begin(); it != routeCounts.end(); ++it)
            if (it->second > busiest->second) busiest = it;
        cout << "Busiest route            : " << busiest->first
             << " (" << busiest->second << " flights)\n";
    }

    cout << "\nAirport activity:\n";
    if (airportCounts.empty()) cout << "No airport activity recorded.\n";
    else {
        for (const auto& item : airportCounts)
            cout << item.first << " : " << item.second << " flight movements\n";
        auto busiestAirport = airportCounts.begin();
        for (auto it = airportCounts.begin(); it != airportCounts.end(); ++it)
            if (it->second > busiestAirport->second) busiestAirport = it;
        cout << "Busiest airport          : " << busiestAirport->first
             << " (" << busiestAirport->second << " movements)\n";
    }
    cout << "================================================\n";
}
