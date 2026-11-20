#ifndef FLIGHT_H
#define FLIGHT_H
#include "models.h"
#include <vector>
void flightMenu(std::vector<Flight>& flights);
void showFlights(const std::vector<Flight>& flights);
int findFlightIndex(const std::vector<Flight>& flights, int id);
#endif
