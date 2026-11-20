#include "gate.h"
#include "algorithms.h"
#include <algorithm>
#include <iostream>
#include <limits>
using namespace std;

static int getNumber(const string& p) {
    int x;
    while (true) {
        cout << p;
        if (cin >> x) return x;
        cout << "Enter a valid number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
static int gateIndex(const vector<Gate>& gates, int id) {
    for (int i = 0; i < (int)gates.size(); ++i)
        if (gates[i].id == id) return i;
    return -1;
}
static bool overlaps(const Flight& a, const Flight& b) {
    return a.departure < b.arrival && b.departure < a.arrival;
}
static bool gateFree(const Gate& gate, const Flight& requested,
                     const vector<Flight>& flights) {
    if (!gate.active) return false;
    for (const auto& other : flights) {
        if (other.id != requested.id && other.gateId == gate.id &&
            other.status != "Cancelled" && overlaps(requested, other))
            return false;
    }
    return true;
}
void showGates(const vector<Gate>& gates, const vector<Flight>& flights) {
    if (gates.empty()) { cout << "No gates configured.\n"; return; }
    cout << "\nGATE  TERMINAL  STATUS  ASSIGNED FLIGHT\n";
    for (const auto& g : gates) {
        int assigned = 0;
        for (const auto& f : flights) if (f.gateId == g.id) assigned = f.id;
        cout << g.id << "     " << g.terminal << "       "
             << (g.active ? "Active" : "Inactive") << "   "
             << (assigned ? to_string(assigned) : "-") << '\n';
    }
}
static void addGate(vector<Gate>& gates) {
    Gate g; g.id = getNumber("Gate ID: ");
    if (gateIndex(gates, g.id) >= 0) { cout << "Gate ID exists.\n"; return; }
    cout << "Terminal: "; cin >> g.terminal;
    g.active = true; gates.push_back(g);
    cout << "Gate added.\n";
}
static void setGateStatus(vector<Gate>& gates, const vector<Flight>& flights) {
    int id = getNumber("Gate ID: "), i = gateIndex(gates, id);
    if (i < 0) { cout << "Gate not found.\n"; return; }
    cout << "1 Activate  2 Deactivate\n";
    int c = getNumber("Choice: ");
    if (c == 1) gates[i].active = true;
    else if (c == 2) {
        for (const auto& f : flights) if (f.gateId == id && f.status != "Cancelled") {
            cout << "Cannot deactivate: flight " << f.id << " still uses this gate.\n";
            return;
        }
        gates[i].active = false;
    } else { cout << "Invalid choice.\n"; return; }
    cout << "Gate status updated.\n";
}
static void assignGate(vector<Gate>& gates, vector<Flight>& flights) {
    int fid = getNumber("Flight ID: ");
    auto f = find_if(flights.begin(), flights.end(),
                     [&](const Flight& x) { return x.id == fid; });
    if (f == flights.end()) { cout << "Flight not found.\n"; return; }
    if (f->status == "Cancelled") { cout << "Cancelled flights do not need a gate.\n"; return; }
    if (f->gateId) { cout << "Flight already has a gate.\n"; return; }
    cout << "1. Automatically select first available gate\n2. Choose a gate manually\n";
    int mode = getNumber("Choice: ");
    if (mode == 1) {
        for (const auto& g : gates) if (gateFree(g, *f, flights)) {
            f->gateId = g.id; cout << "Gate " << g.id << " assigned.\n"; return;
        }
    } else if (mode == 2) {
        int id = getNumber("Gate ID: "), gi = gateIndex(gates, id);
        if (gi < 0) { cout << "Gate not found.\n"; return; }
        if (!gateFree(gates[gi], *f, flights)) {
            cout << "Selected gate is inactive or occupied during this time.\n"; return;
        }
        f->gateId = id; cout << "Gate " << id << " assigned.\n"; return;
    } else { cout << "Invalid choice.\n"; return; }
    cout << "No free gate for this time slot. Add a gate or change the schedule.\n";
}
static void releaseGate(vector<Flight>& flights) {
    int id = getNumber("Flight ID: ");
    for (auto& f : flights) if (f.id == id) {
        if (!f.gateId) { cout << "No gate assigned.\n"; return; }
        f.gateId = 0; cout << "Gate released.\n"; return;
    }
    cout << "Flight not found.\n";
}
static void showAvailableGates(const vector<Gate>& gates,
                               const vector<Flight>& flights) {
    Flight requested;
    requested.id = -1;
    requested.departure = getNumber("Start time (minutes after midnight): ");
    requested.arrival = getNumber("End time (minutes after midnight): ");
    if (requested.departure < 0 || requested.arrival <= requested.departure ||
        requested.arrival > 2879) {
        cout << "Invalid time interval.\n"; return;
    }
    bool found = false;
    cout << "Available gates for this time slot:\n";
    for (const auto& gate : gates) if (gateFree(gate, requested, flights)) {
        cout << "Gate " << gate.id << " (Terminal " << gate.terminal << ")\n";
        found = true;
    }
    if (!found) cout << "No gates are available for this interval.\n";
}
static void removeGate(vector<Gate>& gates, const vector<Flight>& flights) {
    int id = getNumber("Gate ID to remove: "), index = gateIndex(gates, id);
    if (index < 0) { cout << "Gate not found.\n"; return; }
    for (const auto& flight : flights) if (flight.gateId == id) {
        cout << "This gate is assigned. Release its flight before removal.\n"; return;
    }
    gates.erase(gates.begin() + index);
    cout << "Gate removed.\n";
}
static void gateConflictReport(const vector<Flight>& flights) {
    bool conflict = false;
    for (size_t i = 0; i < flights.size(); ++i) {
        if (!flights[i].gateId || flights[i].status == "Cancelled") continue;
        for (size_t j = i + 1; j < flights.size(); ++j)
            if (flights[i].gateId == flights[j].gateId &&
                overlaps(flights[i], flights[j]) && flights[j].status != "Cancelled") {
                cout << "Conflict: gate " << flights[i].gateId
                     << " is assigned to flights " << flights[i].id
                     << " and " << flights[j].id << ".\n";
                conflict = true;
            }
    }
    if (!conflict) cout << "No gate assignment conflicts detected.\n";
}
void gateMenu(vector<Gate>& gates, vector<Flight>& flights) {
    int c;
    do {
        cout << "\n--- GATE MANAGEMENT ---\n"
             << "1. View gates\n2. Add gate\n3. Activate/deactivate gate\n"
             << "4. Assign gate to flight\n5. Release gate\n"
             << "6. Calculate minimum gates needed\n7. Remove gate\n"
             << "8. Check gate conflicts\n9. Find available gates for a time\n0. Back\n";
        c = getNumber("Choice: ");
        if (c == 1) showGates(gates, flights);
        else if (c == 2) addGate(gates);
        else if (c == 3) setGateStatus(gates, flights);
        else if (c == 4) assignGate(gates, flights);
        else if (c == 5) releaseGate(flights);
        else if (c == 6)
            cout << "Minimum gates required by current schedule: "
                 << minimumGates(flights) << '\n';
        else if (c == 7) removeGate(gates, flights);
        else if (c == 8) gateConflictReport(flights);
        else if (c == 9) showAvailableGates(gates, flights);
        else if (c != 0) cout << "Invalid choice.\n";
    } while (c != 0);
}
