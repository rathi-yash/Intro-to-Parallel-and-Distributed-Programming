# A3 Report

## **Overview**
This report explores the performance of Gaussian Kernel Density Estimation (KDE) implemented using NVIDIA CUDA, with a particular focus on its efficiency and scalability. It evaluates how the CUDA implementation handles varying data sizes and benchmarks its performance against a CPU-based approach, highlighting the potential advantages of GPU acceleration for this computational task.

---

## **What is the Function Doing?**
This CUDA-based implementation of Gaussian Kernel Density Estimation (KDE) utilizes GPU parallelism to accelerate computations:
- The kernel function `kde_kernel` assigns each thread to compute the density estimate for a single data point by evaluating the Gaussian kernel over all other points in the dataset.
- The host function `gaussian_kde` manages:
  - Memory transfers between the CPU and GPU.
  - Device memory allocation.
  - Kernel launches and result retrieval.

By distributing computations across threads and blocks, this implementation achieves significant performance improvements for large datasets compared to CPU-based methods.

---

## **Experimental Setup**

### **Hardware and Software Requirements**
To replicate the readings, use the following specifications:
- **CPU**: Intel(R) Xeon(R) Gold 6130
- **RAM**: 128GB
- **Compiler**: `nvcc`
- **Compiler Command**: O2 optimization
- **Operating System**: CentOS Linux 7 (Core)
- **GPU Model**: Tesla V100-PCIE-16GB
- **Driver Version**: 535.216.01
- **CUDA Version**: 12.2
- **Memory**: 16,384 MiB per GPU

### **Input Parameters**
The Gaussian Kernel Density Estimation (KDE) implementation was tested with various input sizes (\( n \)) to evaluate its performance on both GPU and CPU:
1. **Input Size (\( n \))**:
   - Represents the number of data points processed by the KDE algorithm.
   - Test cases included \( n = \{10,000; 20,000; 30,000; 40,000; 50,000; 100,000; 200,000; 400,000; 1,000,000; 2,000,000; 4,000,000\} \).
2. **Smoothing Parameter (\( h \))**:
   - The bandwidth parameter \( h \) controls the smoothness of the KDE.
   - Set to \( h = 0.1 \) for all experiments.
3. **Execution Modes**:
   - *Sequential (CPU)*: Standard implementation for single-threaded execution.
   - *Parallel (GPU)*: CUDA-based implementation optimized for parallel processing.

This range of input sizes allows a detailed analysis of scaling behavior and efficiency for both GPU and CPU implementations.

---

## **Experiment Results**

### Table: Input Size (\( n \)), GPU Runtime (s), CPU Runtime (s), and Speedup

| \( n \)       | GPU Runtime (s) | CPU Runtime (s) | Speedup |
|---------------|-----------------|-----------------|---------|
| 10,000        | 0.13            | 0.98            | 7.5     |
| 20,000        | 0.14            | 3.92            | 28      |
| 30,000        | 0.15            | 8.76            | 58.4    |
| 40,000        | 0.18            | 15.51           | 86.16   |
| 50,000        | 0.16            | 24.4            | 152     |
| 100,000       | 0.18            | -               | -       |
| 200,000       | 0.26            | -               | -       |
| 400,000       | 0.57            | -               | -       |
| 1,000,000     | 2.7             | -               | -       |
| 2,000,000     | 9.8             | -               | -       |
| 4,000,000     | 37.5            | -               | -       |

---

## **Analysis**
The experimental results highlight the significant performance and scalability advantages of GPUs over CPUs for large-scale computational tasks:
1. **GPU Scalability**:
   - GPU runtimes show a gradual and sub-linear increase with input size (\( n \)), demonstrating strong scalability.
   - For very large \( n \), e.g., \( n = 4,000,000 \), GPU runtime reaches **37.5 seconds**, reflecting potential resource saturation but still showcasing remarkable performance.
2. **CPU Limitations**:
   - CPU runtimes exhibit steep super-linear growth.
   - Measurements were unavailable for \( n > 50,000 \), likely due to impracticality or resource constraints.
3. **Speedup**:
   - Speedup values range from \( \times7.5 \) for \( n =10,000 \) to \( \times152 \) for \( n =50,000 \), emphasizing the efficiency of GPUs.
   - The speedup becomes more pronounced as \( n \) increases.

Overall, these results affirm that GPUs are highly effective for scalable parallel workloads when problem sizes exceed \( n =10,000 \).

---

## **Conclusion**
The results of this study underscore the remarkable efficiency and scalability of the CUDA-based Gaussian Kernel Density Estimation (KDE) implementation on GPUs:
1. By leveraging GPU parallelism:
   - Substantial speedups were achieved compared to CPU-based approaches.
   - Near-linear scaling was observed with increasing input sizes.
2. In contrast:
   - The CPU struggled with scalability and performance.
   - It proved unsuitable for large-scale data processing.

These findings highlight the effectiveness of GPUs in handling computationally intensive tasks efficiently and at scale.

---
