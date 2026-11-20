#include "reports.h"
#include <iostream>
#include <map>
using namespace std;

void showReports(const vector<Flight>& flights,
                 const vector<Crew>& crew,
                 const vector<Gate>& gates) {
    int scheduled = 0;
    int delayed = 0;
    int cancelled = 0;
    int assigned = 0;
    long long totalDuration = 0;
    map<string, int> routeCounts;

    // Collect flight statistics and count each active route.
    for (const auto& flight : flights) {
        if (flight.status == "Scheduled") ++scheduled;
        else if (flight.status == "Delayed") ++delayed;
        else if (flight.status == "Cancelled") ++cancelled;

        if (flight.crewId) ++assigned;
        if (flight.status != "Cancelled") {
            totalDuration += flight.arrival - flight.departure;
            ++routeCounts[flight.from + "-" + flight.to];
        }
    }

    int availableCrew = 0;
    int activeGates = 0;
    int usedGates = 0;

    for (const auto& member : crew)
        if (member.available) ++availableCrew;

    for (const auto& gate : gates)
        if (gate.active) ++activeGates;

    for (const auto& gate : gates) {
        for (const auto& flight : flights) {
            if (flight.gateId == gate.id) {
                ++usedGates;
                break;
            }
        }
    }

    cout << "\n========== AIRLINE OPERATIONS REPORT ==========\n";
    cout << "Total flights            : " << flights.size() << '\n';
    cout << "Scheduled flights        : " << scheduled << '\n';
    cout << "Delayed flights          : " << delayed << '\n';
    cout << "Cancelled flights        : " << cancelled << '\n';
    cout << "Crew members             : " << crew.size() << '\n';
    cout << "Available crew           : " << availableCrew << '\n';
    cout << "Crew assigned to flights : " << assigned << '\n';
    cout << "Active gates             : " << activeGates << '\n';
    cout << "Gates currently used     : " << usedGates << '\n';

    int activeFlights = static_cast<int>(flights.size()) - cancelled;
    cout << "Average flight duration  : ";
    if (activeFlights == 0) cout << "N/A\n";
    else cout << totalDuration / activeFlights << " minutes\n";

    // A simple percentage helps compare delays across different schedules.
    cout << "Delay percentage        : ";
    if (activeFlights == 0) cout << "N/A\n";
    else cout << (100.0 * delayed / activeFlights) << "%\n";

    cout << "\nRoute frequency:\n";
    if (routeCounts.empty()) {
        cout << "No active flight data.\n";
    } else {
        for (const auto& item : routeCounts)
            cout << item.first << " : " << item.second << " flight(s)\n";

        // Identify the route with the largest number of scheduled flights.
        auto busiest = routeCounts.begin();
        for (auto it = routeCounts.begin(); it != routeCounts.end(); ++it)
            if (it->second > busiest->second) busiest = it;

        cout << "Busiest route            : " << busiest->first
             << " (" << busiest->second << " flights)\n";
    }
    cout << "================================================\n";
}
