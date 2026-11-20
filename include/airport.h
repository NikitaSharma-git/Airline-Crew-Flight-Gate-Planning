#ifndef AIRPORT_H
#define AIRPORT_H
#include "models.h"
#include <vector>
void airportMenu(std::vector<Airport>& airports, std::vector<Route>& routes);
void showAirports(const std::vector<Airport>& airports, const std::vector<Route>& routes);
#endif
