#ifndef AIRLINE_MODELS_HPP
#define AIRLINE_MODELS_HPP

#include <string>

struct Flight {
    int flightID{};
    std::string airline;
    std::string source;
    std::string destination;
    int departureTime{}; // Minutes after midnight
    int arrivalTime{};   // Minutes after midnight
    int requiredCrew{};
    int assignedGate{};  // 0 means not assigned
    int delayMinutes{};  // Positive = late, negative = early
};

struct Crew {
    int crewID{};
    std::string name;
    int availableFrom{};
    int availableUntil{};
    bool assigned{};
};

struct Gate {
    int gateID{};
    std::string terminal;
    int capacity{};
    bool available{true};
};

inline std::string formatTime(int minutes) {
    const int normalized = ((minutes % 1440) + 1440) % 1440;
    const int hours = normalized / 60;
    const int mins = normalized % 60;

    std::string result;
    result += (hours < 10 ? "0" : "") + std::to_string(hours);
    result += ":";
    result += (mins < 10 ? "0" : "") + std::to_string(mins);
    return result;
}

#endif
