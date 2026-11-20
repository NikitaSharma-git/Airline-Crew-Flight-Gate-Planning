#include "airport.h"
#include <iostream>
#include <algorithm>
#include <limits>
using namespace std;
static int readNumber(const string& p){int x;while(true){cout<<p;if(cin>>x)return x;cout<<"Enter a valid number.\n";cin.clear();cin.ignore(numeric_limits<streamsize>::max(),'\n');}}
static int airportIndex(const vector<Airport>& a,const string& code){for(int i=0;i<(int)a.size();++i)if(a[i].code==code)return i;return -1;}
void showAirports(const vector<Airport>& airports,const vector<Route>& routes){
    cout<<"\nAIRPORTS\n";for(const auto& a:airports)cout<<a.code<<" - "<<a.name<<'\n';
    cout<<"\nROUTES\n";for(const auto& r:routes)cout<<r.from<<" -> "<<r.to<<" : "<<r.distance<<" km\n";
}
static void addAirport(vector<Airport>& airports){Airport a;cout<<"Airport code: ";cin>>a.code;if(airportIndex(airports,a.code)>=0){cout<<"Code already exists.\n";return;}cout<<"Airport name (use underscores instead of spaces): ";cin>>a.name;airports.push_back(a);cout<<"Airport added.\n";}
static void addRoute(vector<Airport>& airports,vector<Route>& routes){Route r;cout<<"From code: ";cin>>r.from;cout<<"To code: ";cin>>r.to;if(airportIndex(airports,r.from)<0||airportIndex(airports,r.to)<0||r.from==r.to){cout<<"Enter two different airports already in the list.\n";return;}r.distance=readNumber("Distance (positive km): ");if(r.distance<=0){cout<<"Distance must be positive.\n";return;}for(auto& old:routes)if(old.from==r.from&&old.to==r.to){old.distance=r.distance;cout<<"Route distance updated.\n";return;}routes.push_back(r);routes.push_back({r.to,r.from,r.distance});cout<<"Two-way route added.\n";}
static void removeAirport(vector<Airport>& airports,vector<Route>& routes){string code;cout<<"Airport code to remove: ";cin>>code;int i=airportIndex(airports,code);if(i<0){cout<<"Airport not found.\n";return;}airports.erase(airports.begin()+i);routes.erase(remove_if(routes.begin(),routes.end(),[&](const Route& r){return r.from==code||r.to==code;}),routes.end());cout<<"Airport and connected routes removed.\n";}
static void showConnections(const vector<Airport>& airports,const vector<Route>& routes){
    string code;cout<<"Airport code: ";cin>>code;
    if(airportIndex(airports,code)<0){cout<<"Airport not found.\n";return;}
    cout<<"Direct connections from "<<code<<":\n";bool found=false;
    for(const auto& route:routes)if(route.from==code){cout<<"  "<<route.to<<" - "<<route.distance<<" km\n";found=true;}
    if(!found)cout<<"No direct routes configured.\n";
}
static void removeRoute(vector<Route>& routes){
    string from,to;cout<<"Origin code: ";cin>>from;cout<<"Destination code: ";cin>>to;size_t oldSize=routes.size();
    routes.erase(remove_if(routes.begin(),routes.end(),[&](const Route& r){return (r.from==from&&r.to==to)||(r.from==to&&r.to==from);}),routes.end());
    if(routes.size()==oldSize)cout<<"Route not found.\n";else cout<<"Both directions of the route were removed.\n";
}
void airportMenu(vector<Airport>& airports,vector<Route>& routes){int c;do{
    cout<<"\n--- AIRPORT AND ROUTE MANAGEMENT ---\n1. View airports and routes\n2. Add airport\n"
        <<"3. Add/update two-way route\n4. Remove airport\n5. View direct connections\n6. Remove route\n0. Back\n";
    c=readNumber("Choice: ");
    if(c==1)showAirports(airports,routes);else if(c==2)addAirport(airports);else if(c==3)addRoute(airports,routes);
    else if(c==4)removeAirport(airports,routes);else if(c==5)showConnections(airports,routes);else if(c==6)removeRoute(routes);
    else if(c!=0)cout<<"Invalid choice.\n";
}while(c!=0);}
