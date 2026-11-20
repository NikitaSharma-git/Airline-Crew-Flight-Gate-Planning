#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <limits>
using namespace std;

struct Flight {
    int id;
    string from, to;
    int departure, arrival;
};

vector<Flight> flights = {
    {101, "DEL", "BOM", 480, 600},
    {102, "DEL", "BLR", 510, 650},
    {103, "DEL", "HYD", 630, 750},
    {104, "DEL", "MAA", 690, 810},
    {105, "DEL", "JAI", 720, 780},
    {106, "DEL", "CCU", 790, 910}
};

string clockTime(int minutes) {
    string h = to_string(minutes / 60), m = to_string(minutes % 60);
    if (h.size() == 1) h = "0" + h;
    if (m.size() == 1) m = "0" + m;
    return h + ":" + m;
}

void showFlights(const vector<Flight>& data) {
    cout << "\nID    FROM   TO     DEPART   ARRIVE\n";
    for (const auto& f : data)
        cout << f.id << "   " << f.from << "    " << f.to << "     "
             << clockTime(f.departure) << "    " << clockTime(f.arrival) << '\n';
}

// 1. Merge Sort: sort flights by departure time.
void mergeFlights(vector<Flight>& a, int left, int mid, int right) {
    vector<Flight> temp;
    int i = left, j = mid + 1;
    while (i <= mid && j <= right) {
        if (a[i].departure <= a[j].departure) temp.push_back(a[i++]);
        else temp.push_back(a[j++]);
    }
    while (i <= mid) temp.push_back(a[i++]);
    while (j <= right) temp.push_back(a[j++]);
    for (int k = 0; k < (int)temp.size(); ++k) a[left + k] = temp[k];
}

void mergeSort(vector<Flight>& a, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(a, left, mid);
    mergeSort(a, mid + 1, right);
    mergeFlights(a, left, mid, right);
}

// 2. Binary Search: find a flight by ID in ID-sorted data.
int binarySearch(const vector<Flight>& a, int id) {
    int low = 0, high = (int)a.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (a[mid].id == id) return mid;
        if (a[mid].id < id) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

// 3. Activity Selection: choose the maximum non-overlapping flights.
void activitySelection() {
    vector<Flight> a = flights;
    sort(a.begin(), a.end(), [](const Flight& x, const Flight& y) {
        return x.arrival < y.arrival;
    });
    int lastArrival = -1, count = 0;
    cout << "\nSelected non-overlapping flights:\n";
    for (const auto& f : a) {
        if (f.departure >= lastArrival) {
            cout << f.id << " (" << clockTime(f.departure) << "-"
                 << clockTime(f.arrival) << ")\n";
            lastArrival = f.arrival;
            ++count;
        }
    }
    cout << "Maximum activities: " << count << '\n';
}

// 4. Interval Partitioning: find the minimum gates needed.
void intervalPartitioning() {
    vector<Flight> a = flights;
    sort(a.begin(), a.end(), [](const Flight& x, const Flight& y) {
        return x.departure < y.departure;
    });
    priority_queue<int, vector<int>, greater<int>> gateEndTimes;
    for (const auto& f : a) {
        if (!gateEndTimes.empty() && gateEndTimes.top() <= f.departure)
            gateEndTimes.pop();
        gateEndTimes.push(f.arrival);
    }
    cout << "Minimum gates required: " << gateEndTimes.size() << '\n';
}

// 5. Dijkstra: shortest route between airports in the sample network.
void dijkstra() {
    const int n = 5, INF = numeric_limits<int>::max() / 4;
    const string airports[n] = {"DEL", "BOM", "BLR", "HYD", "MAA"};
    int graph[n][n] = {
        {0, 4, 2, 0, 0},
        {4, 0, 1, 5, 0},
        {2, 1, 0, 8, 10},
        {0, 5, 8, 0, 2},
        {0, 0, 10, 2, 0}
    };
    vector<int> dist(n, INF), parent(n, -1);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    dist[0] = 0;
    pq.push({0, 0});
    while (!pq.empty()) {
        auto [cost, u] = pq.top();
        pq.pop();
        if (cost != dist[u]) continue;
        for (int v = 0; v < n; ++v) {
            if (graph[u][v] && dist[v] > cost + graph[u][v]) {
                dist[v] = cost + graph[u][v];
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    cout << "Shortest distances from DEL:\n";
    for (int i = 0; i < n; ++i) {
        cout << airports[i] << ": ";
        if (dist[i] == INF) cout << "unreachable";
        else cout << dist[i] << " units";
        cout << '\n';
    }
    cout << "Route to MAA: ";
    vector<int> path;
    for (int v = 4; v != -1; v = parent[v]) path.push_back(v);
    reverse(path.begin(), path.end());
    for (int i = 0; i < (int)path.size(); ++i) {
        if (i) cout << " -> ";
        cout << airports[path[i]];
    }
    cout << '\n';
}

int main() {
    int choice;
    do {
        cout << "\n===== AIRLINE CREW, FLIGHT AND GATE PLANNING =====\n"
             << "1. View flight details\n"
             << "2. Sort flights by departure (Merge Sort)\n"
             << "3. Search flight by ID (Binary Search)\n"
             << "4. Select non-overlapping flights (Activity Selection)\n"
             << "5. Calculate minimum airport gates (Interval Partitioning)\n"
             << "6. Find shortest airport route (Dijkstra)\n"
             << "0. Exit\nChoose: ";
        if (!(cin >> choice)) break;
        if (choice == 1) showFlights(flights);
        else if (choice == 2) {
            vector<Flight> sorted = flights;
            mergeSort(sorted, 0, (int)sorted.size() - 1);
            cout << "\nFlights sorted by departure time:\n";
            showFlights(sorted);
        } else if (choice == 3) {
            int id;
            cout << "Enter flight ID: ";
            cin >> id;
            int index = binarySearch(flights, id);
            if (index == -1) cout << "Flight not found.\n";
            else showFlights(vector<Flight>{flights[index]});
        } else if (choice == 4) activitySelection();
        else if (choice == 5) intervalPartitioning();
        else if (choice == 6) dijkstra();
        else if (choice == 0) cout << "Thank you!\n";
        else cout << "Invalid choice. Try again.\n";
    } while (choice != 0);
    return 0;
}