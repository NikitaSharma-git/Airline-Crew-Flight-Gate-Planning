#include <chrono>
#include <iostream>
#include <string>
#include "advanced_algorithms.hpp"

int main() {
    std::string text;
    for (int i = 0; i < 20000; ++i) text += "DELHI MUMBAI BENGALURU ";
    const std::string pattern = "MUMBAI BENGALURU";
    auto run = [&](const char* name, auto function) {
        auto start = std::chrono::high_resolution_clock::now();
        auto matches = function(text, pattern);
        auto stop = std::chrono::high_resolution_clock::now();
        auto micros = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        std::cout << name << ": " << matches.size() << " matches, " << micros << " microseconds\n";
    };
    run("Naive", advanced::naiveSearch);
    run("Rabin-Karp", advanced::rabinKarpSearch);
    run("KMP", advanced::kmpSearch);
    run("Finite automaton", advanced::finiteAutomatonSearch);
    return 0;
}
