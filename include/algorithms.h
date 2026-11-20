#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include "models.h"
#include <vector>
void algorithmMenu(const std::vector<Flight>& flights, const std::vector<Route>& routes, const std::vector<Airport>& airports);
void mergeSortFlights(std::vector<Flight>& flights, bool byArrival = false);
int binarySearchFlight(const std::vector<Flight>& flights, int id);
int minimumGates(const std::vector<Flight>& flights);
#endif
