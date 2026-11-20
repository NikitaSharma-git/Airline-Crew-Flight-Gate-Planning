#include "crew.h"
#include <iostream>
#include <algorithm>
#include <limits>
using namespace std;
static int number(const string& prompt) { int x; while (true) { cout << prompt; if (cin >> x) return x; cout << "Enter a number.\n"; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); } }
void showCrew(const vector<Crew>& crew) {
    if (crew.empty()) { cout << "No crew records.\n"; return; }
    cout << "\nID   NAME             ROLE          AVAILABILITY\n";
    for (const auto& c : crew) cout << c.id << "   " << c.name << "   " << c.role << "   " << (c.available ? "Available" : "Assigned") << '\n';
}
static int crewIndex(const vector<Crew>& crew, int id) { for (int i=0;i<(int)crew.size();++i) if (crew[i].id==id) return i; return -1; }
static void addCrew(vector<Crew>& crew) {
    Crew c; c.id=number("Crew ID: "); if (crewIndex(crew,c.id)>=0) { cout << "ID already exists.\n"; return; }
    cout << "Name (one word): "; cin >> c.name; cout << "Role (Pilot/Cabin): "; cin >> c.role; c.available=true; crew.push_back(c); cout << "Crew member added.\n";
}
static void removeCrew(vector<Crew>& crew) {
    int id=number("Crew ID to remove: "), i=crewIndex(crew,id); if(i<0) { cout << "Crew member not found.\n"; return; }
    if(!crew[i].available) { cout << "Crew member is assigned to a flight. Unassign first.\n"; return; }
    crew.erase(crew.begin()+i); cout << "Crew member removed.\n";
}
static void assignCrew(vector<Crew>& crew, vector<Flight>& flights) {
    int fid=number("Flight ID: "), cid=number("Crew ID: ");
    auto f=find_if(flights.begin(),flights.end(),[&](const Flight& x){return x.id==fid;});
    int ci=crewIndex(crew,cid);
    if(f==flights.end()||ci<0) { cout << "Flight or crew member not found.\n"; return; }
    if(!crew[ci].available) { cout << "Crew member is already assigned.\n"; return; }
    f->crewId=cid; crew[ci].available=false; cout << "Crew assigned to flight.\n";
}
static void unassignCrew(vector<Crew>& crew, vector<Flight>& flights) {
    int id=number("Flight ID: ");
    for(auto& f:flights) if(f.id==id) {
        if(!f.crewId) { cout << "No crew assigned.\n"; return; }
        int i=crewIndex(crew,f.crewId); if(i>=0) crew[i].available=true; f.crewId=0; cout << "Crew unassigned.\n"; return;
    }
    cout << "Flight not found.\n";
}
static void crewSchedule(const vector<Crew>& crew, const vector<Flight>& flights) {
    int id = number("Crew ID to view: ");
    int index = crewIndex(crew, id);
    if (index < 0) { cout << "Crew member not found.\n"; return; }
    cout << "Schedule for " << crew[index].name << ":\n";
    bool found = false;
    for (const auto& flight : flights) if (flight.crewId == id) {
        cout << "Flight " << flight.id << "  " << flight.from << "-" << flight.to
             << "  " << flight.departure << "-" << flight.arrival << "  " << flight.status << '\n';
        found = true;
    }
    if (!found) cout << "No flights currently assigned.\n";
}
static void crewRoleSummary(const vector<Crew>& crew) {
    int pilots = 0, cabin = 0;
    for (const auto& member : crew) { if (member.role == "Pilot") ++pilots; else if (member.role == "Cabin") ++cabin; }
    cout << "Pilot crew: " << pilots << '\n' << "Cabin crew: " << cabin << '\n'
         << "Other roles: " << crew.size() - pilots - cabin << '\n';
}
void crewMenu(vector<Crew>& crew, vector<Flight>& flights) {
    int choice; do {
        cout << "\n--- CREW MANAGEMENT ---\n"
             << "1. View crew\n2. Add crew member\n3. Remove crew member\n"
             << "4. Assign crew to flight\n5. Unassign crew\n"
             << "6. View crew member schedule\n7. Crew role summary\n0. Back\n";
        choice=number("Choice: ");
        if(choice==1) showCrew(crew); else if(choice==2) addCrew(crew); else if(choice==3) removeCrew(crew);
        else if(choice==4) assignCrew(crew,flights); else if(choice==5) unassignCrew(crew,flights);
        else if(choice==6) crewSchedule(crew,flights); else if(choice==7) crewRoleSummary(crew);
        else if(choice!=0) cout << "Invalid choice.\n";
    } while(choice!=0);
}
