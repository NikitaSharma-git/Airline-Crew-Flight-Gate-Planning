#include "storage.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
static vector<string> split(const string& line){vector<string> fields;stringstream ss(line);string value;while(getline(ss,value,';'))fields.push_back(value);return fields;}
static bool openInput(ifstream& file,const string& path){file.open(path);return file.good();}
void loadAll(vector<Flight>& flights,vector<Crew>& crew,vector<Gate>& gates,vector<Airport>& airports,vector<Route>& routes){
    string line;vector<string> x;ifstream f;
    if(openInput(f,"data/flights.csv")){getline(f,line);while(getline(f,line)){x=split(line);if(x.size()>=8){try{flights.push_back({stoi(x[0]),x[1],x[2],stoi(x[3]),stoi(x[4]),stoi(x[5]),stoi(x[6]),x[7]});}catch(...){cout<<"Skipped invalid flight row.\n";}}}f.close();}
    if(openInput(f,"data/crew.csv")){getline(f,line);while(getline(f,line)){x=split(line);if(x.size()>=4){try{crew.push_back({stoi(x[0]),x[1],x[2],x[3]=="1"});}catch(...){cout<<"Skipped invalid crew row.\n";}}}f.close();}
    if(openInput(f,"data/gates.csv")){getline(f,line);while(getline(f,line)){x=split(line);if(x.size()>=3){try{gates.push_back({stoi(x[0]),x[1],x[2]=="1"});}catch(...){cout<<"Skipped invalid gate row.\n";}}}f.close();}
    if(openInput(f,"data/airports.csv")){getline(f,line);while(getline(f,line)){x=split(line);if(x.size()>=2)airports.push_back({x[0],x[1]});}f.close();}
    if(openInput(f,"data/routes.csv")){getline(f,line);while(getline(f,line)){x=split(line);if(x.size()>=3){try{routes.push_back({x[0],x[1],stoi(x[2])});}catch(...){cout<<"Skipped invalid route row.\n";}}}f.close();}
}
void saveAll(const vector<Flight>& flights,const vector<Crew>& crew,const vector<Gate>& gates,const vector<Airport>& airports,const vector<Route>& routes){
    ofstream f("data/flights.csv");f<<"id;from;to;departure;arrival;crewId;gateId;status\n";
    for(const auto& x:flights)f<<x.id<<';'<<x.from<<';'<<x.to<<';'<<x.departure<<';'<<x.arrival<<';'<<x.crewId<<';'<<x.gateId<<';'<<x.status<<'\n';f.close();
    ofstream c("data/crew.csv");c<<"id;name;role;available\n";for(const auto& x:crew)c<<x.id<<';'<<x.name<<';'<<x.role<<';'<<(x.available?1:0)<<'\n';c.close();
    ofstream g("data/gates.csv");g<<"id;terminal;active\n";for(const auto& x:gates)g<<x.id<<';'<<x.terminal<<';'<<(x.active?1:0)<<'\n';g.close();
    ofstream a("data/airports.csv");a<<"code;name\n";for(const auto& x:airports)a<<x.code<<';'<<x.name<<'\n';a.close();
    ofstream r("data/routes.csv");r<<"from;to;distance\n";for(const auto& x:routes)r<<x.from<<';'<<x.to<<';'<<x.distance<<'\n';r.close();
    cout<<"All records saved to the data folder.\n";
}
