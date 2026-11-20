#include "flight.h"
#include "algorithms.h"
#include <iostream>
#include <limits>
using namespace std;
static int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) return value;
        cout << "Please enter a valid number.\n";
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
static string readWord(const string& prompt) { string value; cout << prompt; cin >> value; return value; }
int findFlightIndex(const vector<Flight>& flights, int id) {
    for (int i = 0; i < static_cast<int>(flights.size()); ++i)
        if (flights[i].id == id) return i;
    return -1;
}
static string displayTime(int minutes) {
    int hour = (minutes / 60) % 24;
    int minute = minutes % 60;
    string h = hour < 10 ? "0" + to_string(hour) : to_string(hour);
    string m = minute < 10 ? "0" + to_string(minute) : to_string(minute);
    return h + ":" + m;
}
void showFlights(const vector<Flight>& flights) {
    if (flights.empty()) { cout << "No flight records available.\n"; return; }
    cout << "\nID    FROM  TO    DEPART  ARRIVE  CREW  GATE  STATUS\n";
    cout << "-----------------------------------------------------\n";
    for (const auto& f : flights) {
        cout << f.id << "   " << f.from << "   " << f.to << "   "
             << displayTime(f.departure) << "   " << displayTime(f.arrival)
             << "   " << (f.crewId ? to_string(f.crewId) : "-")
             << "   " << (f.gateId ? to_string(f.gateId) : "-")
             << "   " << f.status << '\n';
    }
}
static void filterFlightsByStatus(const vector<Flight>& flights) {
    cout << "1 Scheduled  2 Delayed  3 Cancelled\n";
    int choice = readInt("Select status: ");
    if (choice < 1 || choice > 3) { cout << "Invalid status selection.\n"; return; }
    string status = choice == 1 ? "Scheduled" : choice == 2 ? "Delayed" : "Cancelled";
    vector<Flight> filtered;
    for (const auto& flight : flights) if (flight.status == status) filtered.push_back(flight);
    cout << "Flights with status " << status << ":\n";
    showFlights(filtered);
}
static void addFlight(vector<Flight>& flights) {
    Flight f;
    f.id = readInt("Flight ID: ");
    if (findFlightIndex(flights, f.id) >= 0) { cout << "That flight ID already exists.\n"; return; }
    f.from = readWord("Origin airport code: "); f.to = readWord("Destination airport code: ");
    f.departure = readInt("Departure time (minutes after midnight): ");
    f.arrival = readInt("Arrival time (minutes after midnight): ");
    if (f.departure < 0 || f.arrival <= f.departure || f.arrival > 2879) {
        cout << "Invalid time range. Arrival must be after departure.\n"; return;
    }
    flights.push_back(f); cout << "Flight added.\n";
}
static void updateFlight(vector<Flight>& flights) {
    int id = readInt("Flight ID to update: "), i = findFlightIndex(flights, id);
    if (i < 0) { cout << "Flight not found.\n"; return; }
    cout << "1. Change route  2. Change times  3. Change status\n";
    int choice = readInt("Choice: ");
    if (choice == 1) { flights[i].from = readWord("New origin: "); flights[i].to = readWord("New destination: "); }
    else if (choice == 2) {
        int d = readInt("New departure: "), a = readInt("New arrival: ");
        if (d < 0 || a <= d || a > 2879) { cout << "Invalid time range.\n"; return; }
        flights[i].departure = d; flights[i].arrival = a;
    } else if (choice == 3) {
        cout << "1 Scheduled  2 Delayed  3 Cancelled\n"; int s = readInt("Status: ");
        if (s < 1 || s > 3) { cout << "Invalid status.\n"; return; }
        flights[i].status = s == 1 ? "Scheduled" : s == 2 ? "Delayed" : "Cancelled";
    } else cout << "Invalid choice.\n";
}
static void deleteFlight(vector<Flight>& flights) {
    int id = readInt("Flight ID to delete: "), i = findFlightIndex(flights, id);
    if (i < 0) { cout << "Flight not found.\n"; return; }
    flights.erase(flights.begin() + i); cout << "Flight deleted.\n";
}
void flightMenu(vector<Flight>& flights) {
    int choice;
    do {
        cout << "\n--- FLIGHT MANAGEMENT ---\n"
             << "1. View flights\n2. Add flight\n3. Update flight\n"
             << "4. Delete flight\n5. Search flight by ID\n"
             << "6. Sort by departure\n7. Filter by status\n0. Back\n";
        choice = readInt("Choice: ");
        if (choice == 1) showFlights(flights);
        else if (choice == 2) addFlight(flights);
        else if (choice == 3) updateFlight(flights);
        else if (choice == 4) deleteFlight(flights);
        else if (choice == 5) {
            int id = readInt("Flight ID: "), i = findFlightIndex(flights, id);
            if (i < 0) cout << "Flight not found.\n"; else showFlights(vector<Flight>{flights[i]});
        } else if (choice == 6) { mergeSortFlights(flights); showFlights(flights); }
        else if (choice == 7) filterFlightsByStatus(flights);
        else if (choice != 0) cout << "Invalid choice.\n";
    } while (choice != 0);
}
