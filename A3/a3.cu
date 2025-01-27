#include <cuda_runtime.h>
#include <algorithm>
#include <functional>
#include <vector>   
#include <cmath>     
#include "a3.hpp"

#define M_PI 3.14159265358979323846

__global__ void kde_kernel(int n, float h, const float* x, float* y) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (idx < n) {
        double sum = 0.0;
        float xi = x[idx];
        const double inv_h = 1.0 / h;
        const double norm_factor = 1.0 / (sqrtf(2.0 * M_PI) * h * n);
        
        for (int j = 0; j < n; j++) {
            double diff = (xi - x[j]) * inv_h;
            double kernel = expf(-0.5 * diff * diff);
            sum += kernel;
        }

        __syncthreads();
        
        y[idx] = sum * norm_factor;
    }
}


void gaussian_kde(int n, float h, const std::vector<float>& x, std::vector<float>& y) {
    float *d_x, *d_y;
    cudaMalloc(&d_x, n * sizeof(float));
    cudaMalloc(&d_y, n * sizeof(float));
    
    cudaMemcpy(d_x, x.data(), n * sizeof(float), cudaMemcpyHostToDevice);
    
    int threadsPerBlock = 256;
    int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;
    
    kde_kernel<<<blocksPerGrid, threadsPerBlock>>>(n, h, d_x, d_y);
    
    cudaMemcpy(y.data(), d_y, n * sizeof(float), cudaMemcpyDeviceToHost);
    
    cudaFree(d_x);
    cudaFree(d_y);
}
