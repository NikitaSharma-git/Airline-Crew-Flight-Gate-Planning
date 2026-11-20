#include "airport.h"
#include "algorithms.h"
#include "crew.h"
#include "flight.h"
#include "gate.h"
#include "reports.h"
#include "storage.h"
#include <iostream>
#include <limits>
#include <vector>
using namespace std;
static int menuChoice(){int choice;cout<<"\nChoice: ";if(cin>>choice)return choice;cout<<"Please enter a number.\n";cin.clear();cin.ignore(numeric_limits<streamsize>::max(),'\n');return -1;}
int main(){
    vector<Flight> flights;vector<Crew> crew;vector<Gate> gates;vector<Airport> airports;vector<Route> routes;
    loadAll(flights,crew,gates,airports,routes);
    if(flights.empty())cout<<"No flight data loaded. Add records using Flight Management.\n";
    int choice;
    do{
        cout<<"\n================================================\n";
        cout<<"       AIRLINE CREW, FLIGHT AND GATE PLANNER\n";
        cout<<"================================================\n";
        cout<<"1. Flight Management\n2. Crew Management\n3. Gate Management\n4. Airport and Route Management\n";
        cout<<"5. DAA Algorithm Centre (5 algorithms)\n6. Reports and Analytics\n7. Save All Data\n0. Save and Exit\n";
        choice=menuChoice();
        if(choice==1)flightMenu(flights);else if(choice==2)crewMenu(crew,flights);else if(choice==3)gateMenu(gates,flights);
        else if(choice==4)airportMenu(airports,routes);else if(choice==5)algorithmMenu(flights,routes,airports);
        else if(choice==6)showReports(flights,crew,gates);else if(choice==7)saveAll(flights,crew,gates,airports,routes);
        else if(choice==0){saveAll(flights,crew,gates,airports,routes);cout<<"Thank you for using Airline Planner.\n";}
        else cout<<"Invalid menu option. Try again.\n";
    }while(choice!=0);
    return 0;
}
