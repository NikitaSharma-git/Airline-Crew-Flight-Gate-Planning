#ifndef AIRLINE_DIVIDE_CONQUER_HPP
#define AIRLINE_DIVIDE_CONQUER_HPP

#include <iostream>
#include <vector>

struct MaxSubarrayResult { long long sum; int left; int right; };

inline MaxSubarrayResult maximumSubarray(const std::vector<long long>& a, int l, int r) {
    if (l == r) return {a[l], l, r};
    int m = l + (r - l) / 2;
    MaxSubarrayResult x = maximumSubarray(a, l, m);
    MaxSubarrayResult y = maximumSubarray(a, m + 1, r);
    long long sum = 0, ls = a[m]; int li = m;
    for (int i = m; i >= l; --i) { sum += a[i]; if (sum > ls) { ls = sum; li = i; } }
    sum = 0; long long rs = a[m + 1]; int ri = m + 1;
    for (int i = m + 1; i <= r; ++i) { sum += a[i]; if (sum > rs) { rs = sum; ri = i; } }
    MaxSubarrayResult z = {ls + rs, li, ri};
    if (x.sum >= y.sum && x.sum >= z.sum) return x;
    if (y.sum >= x.sum && y.sum >= z.sum) return y;
    return z;
}

inline double fastPower(double base, long long exponent) {
    bool negative = exponent < 0;
    unsigned long long p = negative
        ? static_cast<unsigned long long>(-(exponent + 1)) + 1
        : static_cast<unsigned long long>(exponent);
    double result = 1.0;
    while (p) {
        if (p % 2) result *= base;
        base *= base;
        p /= 2;
    }
    return negative ? 1.0 / result : result;
}

inline int findCelebrity(const std::vector<std::vector<int>>& knows) {
    int n = static_cast<int>(knows.size());
    if (!n) return -1;
    int candidate = 0;
    for (int i = 1; i < n; ++i)
        if (knows[candidate][i]) candidate = i;
    for (int i = 0; i < n; ++i)
        if (i != candidate && (knows[candidate][i] || !knows[i][candidate])) return -1;
    return candidate;
}

using Matrix = std::vector<std::vector<long long>>;

inline Matrix classicalMatrixMultiply(const Matrix& a, const Matrix& b) {
    int n = static_cast<int>(a.size());
    Matrix c(n, std::vector<long long>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < n; ++k)
            for (int j = 0; j < n; ++j) c[i][j] += a[i][k] * b[k][j];
    return c;
}

inline Matrix matrixAdd(const Matrix& a, const Matrix& b, int sign = 1) {
    int n = static_cast<int>(a.size());
    Matrix c(n, std::vector<long long>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) c[i][j] = a[i][j] + sign * b[i][j];
    return c;
}

inline Matrix strassenSquare(const Matrix& a, const Matrix& b) {
    int n = static_cast<int>(a.size());
    if (n == 1) return Matrix(1, std::vector<long long>(1, a[0][0] * b[0][0]));
    int h = n / 2;
    Matrix a11(h, std::vector<long long>(h)), a12 = a11, a21 = a11, a22 = a11;
    Matrix b11 = a11, b12 = a11, b21 = a11, b22 = a11;
    for (int i = 0; i < h; ++i) for (int j = 0; j < h; ++j) {
        a11[i][j] = a[i][j]; a12[i][j] = a[i][j+h];
        a21[i][j] = a[i+h][j]; a22[i][j] = a[i+h][j+h];
        b11[i][j] = b[i][j]; b12[i][j] = b[i][j+h];
        b21[i][j] = b[i+h][j]; b22[i][j] = b[i+h][j+h];
    }
    Matrix m1 = strassenSquare(matrixAdd(a11,a22), matrixAdd(b11,b22));
    Matrix m2 = strassenSquare(matrixAdd(a21,a22), b11);
    Matrix m3 = strassenSquare(a11, matrixAdd(b12,b22,-1));
    Matrix m4 = strassenSquare(a22, matrixAdd(b21,b11,-1));
    Matrix m5 = strassenSquare(matrixAdd(a11,a12), b22);
    Matrix m6 = strassenSquare(matrixAdd(a21,a11,-1), matrixAdd(b11,b12));
    Matrix m7 = strassenSquare(matrixAdd(a12,a22,-1), matrixAdd(b21,b22));
    Matrix c11 = matrixAdd(matrixAdd(matrixAdd(m1,m4),m5,-1),m7);
    Matrix c12 = matrixAdd(m3,m5);
    Matrix c21 = matrixAdd(m2,m4);
    Matrix c22 = matrixAdd(matrixAdd(matrixAdd(m1,m2,-1),m3),m6);
    Matrix c(n, std::vector<long long>(n));
    for (int i = 0; i < h; ++i) for (int j = 0; j < h; ++j) {
        c[i][j]=c11[i][j]; c[i][j+h]=c12[i][j];
        c[i+h][j]=c21[i][j]; c[i+h][j+h]=c22[i][j];
    }
    return c;
}

inline Matrix strassenMatrixMultiply(const Matrix& a, const Matrix& b) {
    int n = static_cast<int>(a.size()), p = 1;
    while (p < n) p *= 2;
    Matrix x(p, std::vector<long long>(p,0)), y(p, std::vector<long long>(p,0));
    for (int i=0;i<n;++i) for (int j=0;j<n;++j) { x[i][j]=a[i][j]; y[i][j]=b[i][j]; }
    Matrix z = strassenSquare(x,y), result(n,std::vector<long long>(n));
    for (int i=0;i<n;++i) for (int j=0;j<n;++j) result[i][j]=z[i][j];
    return result;
}

inline void printMatrix(const Matrix& matrix) {
    for (const auto& row : matrix) {
        for (std::size_t j=0;j<row.size();++j) {
            if (j) std::cout << " ";
            std::cout << row[j];
        }
        std::cout << '\n';
    }
}
#endif
