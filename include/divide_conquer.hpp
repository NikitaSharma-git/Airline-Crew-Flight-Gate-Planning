#ifndef AIRLINE_DIVIDE_CONQUER_HPP
#define AIRLINE_DIVIDE_CONQUER_HPP

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct MaxSubarrayResult {
    long long sum;
    int left;
    int right;
};

inline MaxSubarrayResult maximumSubarray(const std::vector<long long>& values, int left, int right) {
    if (left == right) return {values[left], left, right};

    int mid = left + (right - left) / 2;
    MaxSubarrayResult first = maximumSubarray(values, left, mid);
    MaxSubarrayResult second = maximumSubarray(values, mid + 1, right);

    long long sum = 0;
    long long bestLeftSum = values[mid];
    int bestLeft = mid;
    for (int i = mid; i >= left; --i) {
        sum += values[i];
        if (sum > bestLeftSum) {
            bestLeftSum = sum;
            bestLeft = i;
        }
    }

    sum = 0;
    long long bestRightSum = values[mid + 1];
    int bestRight = mid + 1;
    for (int i = mid + 1; i <= right; ++i) {
        sum += values[i];
        if (sum > bestRightSum) {
            bestRightSum = sum;
            bestRight = i;
        }
    }

    MaxSubarrayResult crossing = {bestLeftSum + bestRightSum, bestLeft, bestRight};
    if (first.sum >= second.sum && first.sum >= crossing.sum) return first;
    if (second.sum >= first.sum && second.sum >= crossing.sum) return second;
    return crossing;
}

inline int findCelebrity(const std::vector<std::vector<int>>& routes) {
    const int n = static_cast<int>(routes.size());
    if (n == 0) return -1;

    int candidate = 0;
    for (int airport = 1; airport < n; ++airport) {
        if (routes[candidate][airport] == 1) candidate = airport;
    }

    for (int airport = 0; airport < n; ++airport) {
        if (airport == candidate) continue;
        if (routes[candidate][airport] != 0 || routes[airport][candidate] != 1) {
            return -1;
        }
    }
    return candidate;
}

using Matrix = std::vector<std::vector<long long>>;

inline Matrix matrixAdd(const Matrix& a, const Matrix& b, int sign = 1) {
    int n = static_cast<int>(a.size());
    Matrix result(n, std::vector<long long>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            result[i][j] = a[i][j] + sign * b[i][j];
    return result;
}

inline Matrix strassenSquare(const Matrix& a, const Matrix& b) {
    int n = static_cast<int>(a.size());
    if (n == 1) return Matrix(1, std::vector<long long>(1, a[0][0] * b[0][0]));

    int half = n / 2;
    Matrix a11(half, std::vector<long long>(half)), a12 = a11, a21 = a11, a22 = a11;
    Matrix b11 = a11, b12 = a11, b21 = a11, b22 = a11;

    for (int i = 0; i < half; ++i) {
        for (int j = 0; j < half; ++j) {
            a11[i][j] = a[i][j];
            a12[i][j] = a[i][j + half];
            a21[i][j] = a[i + half][j];
            a22[i][j] = a[i + half][j + half];
            b11[i][j] = b[i][j];
            b12[i][j] = b[i][j + half];
            b21[i][j] = b[i + half][j];
            b22[i][j] = b[i + half][j + half];
        }
    }

    Matrix m1 = strassenSquare(matrixAdd(a11, a22), matrixAdd(b11, b22));
    Matrix m2 = strassenSquare(matrixAdd(a21, a22), b11);
    Matrix m3 = strassenSquare(a11, matrixAdd(b12, b22, -1));
    Matrix m4 = strassenSquare(a22, matrixAdd(b21, b11, -1));
    Matrix m5 = strassenSquare(matrixAdd(a11, a12), b22);
    Matrix m6 = strassenSquare(matrixAdd(a21, a11, -1), matrixAdd(b11, b12));
    Matrix m7 = strassenSquare(matrixAdd(a12, a22, -1), matrixAdd(b21, b22));

    Matrix c11 = matrixAdd(matrixAdd(matrixAdd(m1, m4), m5, -1), m7);
    Matrix c12 = matrixAdd(m3, m5);
    Matrix c21 = matrixAdd(m2, m4);
    Matrix c22 = matrixAdd(matrixAdd(matrixAdd(m1, m2, -1), m3), m6);

    Matrix result(n, std::vector<long long>(n, 0));
    for (int i = 0; i < half; ++i) {
        for (int j = 0; j < half; ++j) {
            result[i][j] = c11[i][j];
            result[i][j + half] = c12[i][j];
            result[i + half][j] = c21[i][j];
            result[i + half][j + half] = c22[i][j];
        }
    }
    return result;
}

inline Matrix strassenMatrixMultiply(const Matrix& a, const Matrix& b) {
    int n = static_cast<int>(a.size());
    if (n == 0 || static_cast<int>(b.size()) != n) return Matrix();

    int paddedSize = 1;
    while (paddedSize < n) paddedSize *= 2;

    Matrix paddedA(paddedSize, std::vector<long long>(paddedSize, 0));
    Matrix paddedB(paddedSize, std::vector<long long>(paddedSize, 0));

    for (int i = 0; i < n; ++i) {
        if (static_cast<int>(a[i].size()) != n || static_cast<int>(b[i].size()) != n) return Matrix();
        for (int j = 0; j < n; ++j) {
            paddedA[i][j] = a[i][j];
            paddedB[i][j] = b[i][j];
        }
    }

    Matrix paddedResult = strassenSquare(paddedA, paddedB);
    Matrix result(n, std::vector<long long>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            result[i][j] = paddedResult[i][j];
    return result;
}

inline void printMatrix(const Matrix& matrix, const std::vector<std::string>& airportNames) {
    std::cout << "          ";
    for (const auto& name : airportNames) std::cout << name << "  ";
    std::cout << '\n';
    for (std::size_t i = 0; i < matrix.size(); ++i) {
        std::cout << airportNames[i] << "  ";
        for (long long value : matrix[i]) std::cout << value << "       ";
        std::cout << '\n';
    }
}

#endif
