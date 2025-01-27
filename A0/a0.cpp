/*  YOUR_FIRST_NAME
 *  YOUR_LAST_NAME
 *  YOUR_UBIT_NAME
 */

#include <chrono>
#include <cmath>
#include <iostream>
#include <numbers>
#include <vector>


inline double K(double x) {
    const double d = 1.0 / std::sqrt(2 * std::numbers::pi);
    return d * std::exp(-(x * x) / 2);
} // K


// sequential Gaussian KDE
void gaussian_kde(double h, int k, const std::vector<double>& x, std::vector<double>& y) {
    int n = x.size();

    for (int i = 0; i < n; ++i) {
        int f = std::max(0, i - k);
        int l = std::min(n, i + k);

        double S = 0.0;

        for (int j = f; j < l; ++j) S += K(x[i] - x[j]);
        y[i] = S / (2 * k * h);
    } // for i
} // gaussian_kde

// OMP parallel Gaussian KDE
void omp_gaussian_kde(double h, int k, const std::vector<double>& x, std::vector<double>& y) {
    int n = x.size();

    #pragma omp parallel for default(none) shared(x, y, n, k, h)
    for (int i = 0; i < n; ++i) {
        int f = std::max(0, i - k);
        int l = std::min(n, i + k);

        double S = 0.0;

        for (int j = f; j < l; ++j) S += K(x[i] - x[j]);
        y[i] = S / (2 * k * h);
    } // for i
} // omp_gaussian_kde


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "usage: " << argv[0] << "n k" << std::endl;
        return -1;
    }

    int n = std::stoi(argv[1]);
    int k = std::stoi(argv[2]);

    if ((n < 2) || (n <= k) || (k < 2)) {
        std::cout << "error: incorrect n or k" << std::endl;
        return -1;
    }

    double h = 0.001;

    std::vector<double> x(n);
    std::vector<double> y(n);

    auto t0 = std::chrono::steady_clock::now();

    // HERE WE GO!!!
    //gaussian_kde(h, k, x, y);
    omp_gaussian_kde(h, k, x, y);

    auto t1 = std::chrono::steady_clock::now();

    // report time in seconds
    std::chrono::duration<double> dT = t1 - t0;
    std::cout << dT.count() << std::endl;

    return 0;
} // main
