#ifndef AIRLINE_SORTING_SEARCHING_HPP
#define AIRLINE_SORTING_SEARCHING_HPP

#include <vector>
#include "models.hpp"

// Merge Sort: sorts flights by flight ID in ascending order.
inline void mergeFlights(std::vector<Flight>& flights, int left, int middle,
                         int right, unsigned long long& comparisons) {
    const int leftSize = middle - left + 1;
    const int rightSize = right - middle;
    std::vector<Flight> leftPart(leftSize);
    std::vector<Flight> rightPart(rightSize);

    for (int i = 0; i < leftSize; ++i) leftPart[i] = flights[left + i];
    for (int j = 0; j < rightSize; ++j) rightPart[j] = flights[middle + 1 + j];

    int i = 0;
    int j = 0;
    int k = left;

    while (i < leftSize && j < rightSize) {
        ++comparisons;
        if (leftPart[i].flightID <= rightPart[j].flightID) {
            flights[k++] = leftPart[i++];
        } else {
            flights[k++] = rightPart[j++];
        }
    }

    while (i < leftSize) flights[k++] = leftPart[i++];
    while (j < rightSize) flights[k++] = rightPart[j++];
}

inline void mergeSortFlights(std::vector<Flight>& flights, int left, int right,
                             unsigned long long& comparisons) {
    if (left >= right) return;

    const int middle = left + (right - left) / 2;
    mergeSortFlights(flights, left, middle, comparisons);
    mergeSortFlights(flights, middle + 1, right, comparisons);
    mergeFlights(flights, left, middle, right, comparisons);
}

// Quick Sort: Lomuto partition scheme, sorting by flight ID.
inline int partitionFlights(std::vector<Flight>& flights, int low, int high,
                            unsigned long long& comparisons) {
    const int pivot = flights[high].flightID;
    int smallerIndex = low - 1;

    for (int current = low; current < high; ++current) {
        ++comparisons;
        if (flights[current].flightID <= pivot) {
            ++smallerIndex;
            Flight temporary = flights[smallerIndex];
            flights[smallerIndex] = flights[current];
            flights[current] = temporary;
        }
    }

    Flight temporary = flights[smallerIndex + 1];
    flights[smallerIndex + 1] = flights[high];
    flights[high] = temporary;
    return smallerIndex + 1;
}

inline void quickSortFlights(std::vector<Flight>& flights, int low, int high,
                             unsigned long long& comparisons) {
    if (low < high) {
        const int pivotIndex = partitionFlights(flights, low, high, comparisons);
        quickSortFlights(flights, low, pivotIndex - 1, comparisons);
        quickSortFlights(flights, pivotIndex + 1, high, comparisons);
    }
}

// Iterative Binary Search. The flight list must be sorted by flight ID.
inline int iterativeBinarySearch(const std::vector<Flight>& flights, int targetID,
                                 unsigned long long& comparisons) {
    int low = 0;
    int high = static_cast<int>(flights.size()) - 1;

    while (low <= high) {
        const int middle = low + (high - low) / 2;
        ++comparisons;

        if (flights[middle].flightID == targetID) return middle;
        if (flights[middle].flightID < targetID) low = middle + 1;
        else high = middle - 1;
    }
    return -1;
}

// Recursive Binary Search. The flight list must be sorted by flight ID.
inline int recursiveBinarySearch(const std::vector<Flight>& flights, int targetID,
                                 int low, int high,
                                 unsigned long long& comparisons) {
    if (low > high) return -1;

    const int middle = low + (high - low) / 2;
    ++comparisons;

    if (flights[middle].flightID == targetID) return middle;
    if (flights[middle].flightID < targetID) {
        return recursiveBinarySearch(flights, targetID, middle + 1, high, comparisons);
    }
    return recursiveBinarySearch(flights, targetID, low, middle - 1, comparisons);
}

#endif
