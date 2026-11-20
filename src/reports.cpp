#include "reports.h"
#include <iostream>
#include <map>
using namespace std;
void showReports(const vector<Flight>& flights,const vector<Crew>& crew,const vector<Gate>& gates){
    int scheduled=0,delayed=0,cancelled=0,assigned=0;long long totalDuration=0;map<string,int> routeCounts;
    for(const auto& f:flights){
        if(f.status=="Scheduled")++scheduled;else if(f.status=="Delayed")++delayed;else if(f.status=="Cancelled")++cancelled;
        if(f.crewId)++assigned;
        if(f.status!="Cancelled"){totalDuration+=f.arrival-f.departure;++routeCounts[f.from+"-"+f.to];}
    }
    int availableCrew=0,activeGates=0,usedGates=0;
    for(const auto& c:crew)if(c.available)++availableCrew;
    for(const auto& g:gates)if(g.active)++activeGates;
    for(const auto& g:gates)for(const auto& f:flights)if(f.gateId==g.id){++usedGates;break;}
    cout<<"\n========== AIRLINE OPERATIONS REPORT ==========\n";
    cout<<"Total flights           : "<<flights.size()<<'\n';
    cout<<"Scheduled flights       : "<<scheduled<<'\n';
    cout<<"Delayed flights         : "<<delayed<<'\n';
    cout<<"Cancelled flights       : "<<cancelled<<'\n';
    cout<<"Crew members            : "<<crew.size()<<'\n';
    cout<<"Available crew          : "<<availableCrew<<'\n';
    cout<<"Crew assigned to flights: "<<assigned<<'\n';
    cout<<"Active gates            : "<<activeGates<<'\n';
    cout<<"Gates currently used    : "<<usedGates<<'\n';
    int activeFlights=(int)flights.size()-cancelled;
    cout<<"Average flight duration : ";
    if(activeFlights==0)cout<<"N/A\n";else cout<<totalDuration/activeFlights<<" minutes\n";
    cout<<"\nRoute frequency:\n";
    if(routeCounts.empty())cout<<"No active flight data.\n";
    for(const auto& item:routeCounts)cout<<item.first<<" : "<<item.second<<" flight(s)\n";
    cout<<"================================================\n";
}
