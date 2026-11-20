#ifndef GATE_H
#define GATE_H
#include "models.h"
#include <vector>
void gateMenu(std::vector<Gate>& gates, std::vector<Flight>& flights);
void showGates(const std::vector<Gate>& gates, const std::vector<Flight>& flights);
#endif
