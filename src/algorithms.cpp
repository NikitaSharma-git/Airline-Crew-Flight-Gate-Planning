#include "algorithms.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
using namespace std;
static void mergeParts(vector<Flight>& a,int left,int mid,int right,bool byArrival){
    vector<Flight> temp;int i=left,j=mid+1;
    while(i<=mid&&j<=right){bool take=byArrival?a[i].arrival<=a[j].arrival:a[i].departure<=a[j].departure;if(take)temp.push_back(a[i++]);else temp.push_back(a[j++]);}
    while(i<=mid)temp.push_back(a[i++]);while(j<=right)temp.push_back(a[j++]);
    for(int k=0;k<(int)temp.size();++k)a[left+k]=temp[k];
}
static void mergePartsId(vector<Flight>& a,int left,int mid,int right){
    vector<Flight> temp;int i=left,j=mid+1;
    while(i<=mid&&j<=right){if(a[i].id<=a[j].id)temp.push_back(a[i++]);else temp.push_back(a[j++]);}
    while(i<=mid)temp.push_back(a[i++]);while(j<=right)temp.push_back(a[j++]);
    for(int k=0;k<(int)temp.size();++k)a[left+k]=temp[k];
}
static void sortRange(vector<Flight>& a,int l,int r,bool byArrival){if(l>=r)return;int m=l+(r-l)/2;sortRange(a,l,m,byArrival);sortRange(a,m+1,r,byArrival);mergeParts(a,l,m,r,byArrival);}
static void sortIdRange(vector<Flight>& a,int l,int r){if(l>=r)return;int m=l+(r-l)/2;sortIdRange(a,l,m);sortIdRange(a,m+1,r);mergePartsId(a,l,m,r);}
void mergeSortFlights(vector<Flight>& flights,bool byArrival){if(!flights.empty())sortRange(flights,0,(int)flights.size()-1,byArrival);}
static void mergeSortById(vector<Flight>& flights){if(!flights.empty())sortIdRange(flights,0,(int)flights.size()-1);}
int binarySearchFlight(const vector<Flight>& flights,int id){int low=0,high=(int)flights.size()-1;while(low<=high){int mid=low+(high-low)/2;if(flights[mid].id==id)return mid;if(flights[mid].id<id)low=mid+1;else high=mid-1;}return -1;}
static void printFlightLine(const Flight& f){cout<<"Flight "<<f.id<<"  "<<f.from<<"-"<<f.to<<"  "<<f.departure<<" to "<<f.arrival<<'\n';}
static void demonstrateMergeSort(const vector<Flight>& flights){vector<Flight> a=flights;mergeSortFlights(a);cout<<"\nFlights sorted by departure time:\n";for(const auto& f:a)printFlightLine(f);}
static void demonstrateBinarySearch(const vector<Flight>& flights){if(flights.empty()){cout<<"No flights to search.\n";return;}int id;cout<<"Enter flight ID: ";cin>>id;vector<Flight> a=flights;mergeSortById(a);int pos=binarySearchFlight(a,id);if(pos<0)cout<<"Flight not found.\n";else{cout<<"Flight found: ";printFlightLine(a[pos]);}}
static void activitySelection(const vector<Flight>& flights){
    if(flights.empty()){cout<<"No flights available.\n";return;}
    vector<Flight> a=flights;mergeSortFlights(a,true);int finish=-1,count=0;
    cout<<"\nSelected non-overlapping flight activities:\n";
    for(const auto& f:a)if(f.departure>=finish&&f.status!="Cancelled"){printFlightLine(f);finish=f.arrival;++count;}
    cout<<"Maximum selected activities: "<<count<<'\n';
}
int minimumGates(const vector<Flight>& flights){
    vector<Flight> a;for(const auto& f:flights)if(f.status!="Cancelled")a.push_back(f);
    mergeSortFlights(a);priority_queue<int,vector<int>,greater<int>> ends;
    for(const auto& f:a){if(!ends.empty()&&ends.top()<=f.departure)ends.pop();ends.push(f.arrival);}
    return (int)ends.size();
}
static void demonstrateInterval(const vector<Flight>& flights){cout<<"Minimum gates required for current non-cancelled schedule: "<<minimumGates(flights)<<'\n';}
static void dijkstra(const vector<Airport>& airports,const vector<Route>& routes){
    if(airports.empty()){cout<<"No airports configured.\n";return;}
    cout<<"Available airports: ";for(const auto& a:airports)cout<<a.code<<' ';cout<<'\n';
    string source,target;cout<<"Source airport: ";cin>>source;cout<<"Destination airport: ";cin>>target;
    int n=(int)airports.size(),s=-1,t=-1;
    for(int i=0;i<n;++i){if(airports[i].code==source)s=i;if(airports[i].code==target)t=i;}
    if(s<0||t<0){cout<<"Unknown airport code.\n";return;}
    vector<vector<pair<int,int>>> graph(n);
    for(const auto& r:routes){int u=-1,v=-1;for(int i=0;i<n;++i){if(airports[i].code==r.from)u=i;if(airports[i].code==r.to)v=i;}if(u>=0&&v>=0&&r.distance>0)graph[u].push_back({v,r.distance});}
    const int INF=numeric_limits<int>::max()/4;vector<int> dist(n,INF),parent(n,-1);
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
    dist[s]=0;pq.push({0,s});
    while(!pq.empty()){
        int cost=pq.top().first,u=pq.top().second;pq.pop();
        if(cost!=dist[u])continue;
        for(auto edge:graph[u]){int v=edge.first,w=edge.second;if(dist[v]>cost+w){dist[v]=cost+w;parent[v]=u;pq.push({dist[v],v});}}
    }
    if(dist[t]==INF){cout<<"No route connects these airports.\n";return;}
    vector<int> path;for(int v=t;v!=-1;v=parent[v])path.push_back(v);reverse(path.begin(),path.end());
    cout<<"Shortest route: ";for(int i=0;i<(int)path.size();++i){if(i)cout<<" -> ";cout<<airports[path[i]].code;}
    cout<<"\nTotal distance: "<<dist[t]<<" km\n";
}
void algorithmMenu(const vector<Flight>& flights,const vector<Route>& routes,const vector<Airport>& airports){int c;do{
    cout<<"\n--- DAA ALGORITHM CENTRE ---\n1. Merge Sort - sort flights by departure\n"
        <<"2. Binary Search - find flight by ID\n3. Activity Selection - choose non-overlapping flights\n"
        <<"4. Interval Partitioning - minimum gates\n5. Dijkstra - shortest airport route\n0. Back\n";
    cout<<"Choice: ";if(!(cin>>c)){cin.clear();cin.ignore(numeric_limits<streamsize>::max(),'\n');c=-1;}
    if(c==1)demonstrateMergeSort(flights);else if(c==2)demonstrateBinarySearch(flights);else if(c==3)activitySelection(flights);
    else if(c==4)demonstrateInterval(flights);else if(c==5)dijkstra(airports,routes);else if(c!=0)cout<<"Invalid choice.\n";
}while(c!=0);}
