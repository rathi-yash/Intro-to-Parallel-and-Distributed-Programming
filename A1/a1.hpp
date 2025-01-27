/*  Yash
 *  Rathi
 *  YRATHI2
 */

#ifndef A1_HPP
#define A1_HPP

#include <vector>
#include <omp.h>
#include <iostream>

void filter2d(long long int n, long long int m, const std::vector<float>& K, std::vector<float>& A) {
    int p = omp_get_max_threads();
    int block_size = (n + p - 1) / p;
    int chunk_size = (n + block_size - 1) / block_size;

    std::vector<std::vector<float>> top_memo(chunk_size, std::vector<float>(m, 0));
    std::vector<std::vector<float>> bottom_memo(chunk_size, std::vector<float>(m, 0));

    for (int j = 0; j < m; j++) {
        top_memo[0][j] = A[j];
        bottom_memo[chunk_size - 1][j] = A[(n - 1) * m + j];
    }

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();

        // Store the previous row
        std::vector<float> prev_buffer(m, 0);

        // boundary rows for top_memo and bottom_memo
        #pragma omp for schedule(static)
        for (long long int i = 1; i < n - 1; i++) {
            int block_index = i / block_size;

            if (i % block_size == 0 && i > 0) {
                for (int j = 0; j < m; j++) {
                    top_memo[block_index][j] = A[(i - 1) * m + j];
                }
            }

            if ((i + 1) % block_size == 0 || (i == n - 2)) {
                for (int j = 0; j < m; j++) {
                    bottom_memo[block_index][j] = A[(i + 1) * m + j];
                }
            }
        }

        #pragma omp barrier

        // computation
        #pragma omp for schedule(static)
        for (int block_index = 0; block_index < chunk_size; block_index++) {
            int block_start = block_index * block_size;
            int block_end = std::min(block_start + block_size, static_cast<int>(n - 1));

            for (long long int i = block_start; i < block_end; i++) {

                if (i == block_start && block_index > 0) {
                    for (int j = 0; j < m; j++) {
                        prev_buffer[j] = top_memo[block_index][j];
                    }
                }

                std::vector<float> output_buffer(m, 0);

                for (long long int j = 1; j < m - 1; ++j) {
                    float submatrix[3][3];

                    for (int ki = -1; ki <= 1; ++ki) {
                        for (int kj = -1; kj <= 1; ++kj) {
                            if (ki == -1) {
                                submatrix[ki + 1][kj + 1] = prev_buffer[j + kj];
                            } else if (ki == 0) {
                                submatrix[ki + 1][kj + 1] = A[i * m + (j + kj)];
                            } else {
                                if (i + 1 < n - 1 && ((i + 1) % block_size != 0)) {
                                    submatrix[ki + 1][kj + 1] = A[(i + 1) * m + (j + kj)];
                                } else {
                                    submatrix[ki + 1][kj + 1] = bottom_memo[block_index][j + kj];
                                }
                            }
                        }
                    }

                    float Z[3][3] = {{0}};
                    for (int row = 0; row < 3; row++) {
                        for (int col = 0; col < 3; col++) {
                            for (int k = 0; k < 3; k++) {
                                Z[row][col] += submatrix[row][k] * K[k * 3 + col];
                            }
                        }
                    }

                    float sum = 0.0f;
                    for (int a = 0; a < 3; a++) {
                        for (int b = 0; b < 3; b++) {
                            sum += Z[a][b];
                        }
                    }
                    output_buffer[j] = sum;
                }

                for (int j = 0; j < m; j++) {
                    prev_buffer[j] = A[i * m + j];
                }

                for (long long int j = 1; j < m - 1; j++) {
                    A[i * m + j] = output_buffer[j];
                }
            }
        }
    }

    // Copy the first row from top_memo back to A to ensure it remains unchanged
    for (int j = 0; j < m; j++) {
        A[j] = top_memo[0][j];
    }
}

#endif // A1_HPP
