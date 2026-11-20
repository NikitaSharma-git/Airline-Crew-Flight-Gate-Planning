#ifndef MODELS_H
#define MODELS_H
#include <string>
struct Flight { int id{}; std::string from, to; int departure{}, arrival{}; int crewId{}; int gateId{}; std::string status{"Scheduled"}; };
struct Crew { int id{}; std::string name, role; bool available{true}; };
struct Gate { int id{}; std::string terminal; bool active{true}; };
struct Airport { std::string code, name; };
struct Route { std::string from, to; int distance{}; };
#endif
