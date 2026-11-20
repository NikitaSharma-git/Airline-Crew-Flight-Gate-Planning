#ifndef STORAGE_H
#define STORAGE_H
#include "models.h"
#include <vector>
void loadAll(std::vector<Flight>& flights, std::vector<Crew>& crew, std::vector<Gate>& gates, std::vector<Airport>& airports, std::vector<Route>& routes);
void saveAll(const std::vector<Flight>& flights, const std::vector<Crew>& crew, const std::vector<Gate>& gates, const std::vector<Airport>& airports, const std::vector<Route>& routes);
#endif
