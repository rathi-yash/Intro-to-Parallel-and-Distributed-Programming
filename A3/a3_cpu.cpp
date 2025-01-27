#include <algorithm>
#include <vector>
#include <cmath>
#include "a3.hpp"

#define M_PI 3.14159265358979323846


void gaussian_kde_cpu(int n, float h, const std::vector<float>& x, std::vector<float>& y) {
    const double inv_h = 1.0 / h;
    const double norm_factor = 1.0 / (sqrt(2.0 * M_PI) * h * n);

    for (int idx = 0; idx < n; idx++) {
        double sum = 0.0;
        float xi = x[idx];

        for (int j = 0; j < n; j++) {
            double diff = (xi - x[j]) * inv_h;
            double kernel = exp(-0.5 * diff * diff);
            sum += kernel;
        }

        y[idx] = sum * norm_factor;
    }
}

