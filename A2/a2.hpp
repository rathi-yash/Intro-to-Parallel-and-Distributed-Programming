/*  Yash
 *  Rathi
 *  YRATHI2
 */

#ifndef A2_HPP
#define A2_HPP

#include <vector>
#include <mpi.h>
#include <numeric>
#include <algorithm>

void isort(std::vector<short int>& Xi, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    const int RANGE = 65536;
    std::vector<int> local_bucket(RANGE, 0);
    for (int i = 0; i < Xi.size(); i++) {
        local_bucket[Xi[i] + 32768]++;
    }

    std::vector<int> global_bucket(RANGE, 0);
    long long total_elements = 0; // Initialize here

    MPI_Allreduce(local_bucket.data(), global_bucket.data(), RANGE, MPI_INT, MPI_SUM, comm);
    
    for (int count : global_bucket) {
        total_elements += count;
    }

    int target_elements_per_proc = (total_elements + size - 1) / size;

    std::vector<int> proc_assignments(RANGE, -1);
    std::vector<int> elements_per_proc(size, 0);
    
    int current_proc = 0;
    int current_count = 0;

    for (int i = 0; i < RANGE; i++) {
        if (current_proc < size - 1 && 
            (current_count + global_bucket[i] > target_elements_per_proc || 
                (i == RANGE - 1 && elements_per_proc[current_proc + 1] == 0))) {
            current_proc++;
            current_count = 0;
        }
        proc_assignments[i] = current_proc;
        elements_per_proc[current_proc] += global_bucket[i];
        current_count += global_bucket[i];
    }

    // Calculate the size for local_sorted_data
    long long local_size = 0;
    for (int i = 0; i < RANGE; i++) {
        if (proc_assignments[i] == rank) {
            local_size += global_bucket[i];
        }
    }

    // Resize the vector to the calculated size
    std::vector<short int> local_sorted_data;
    local_sorted_data.resize(static_cast<size_t>(local_size));

    // Fill the resized vector
    size_t index = 0;
    for (int i = 0; i < RANGE; i++) {
        if (proc_assignments[i] == rank) {
            std::generate(local_sorted_data.begin() + index, 
                        local_sorted_data.begin() + index + global_bucket[i], 
                        [value = i - 32768]() mutable { return value; });
            index += global_bucket[i];
        }
    }


    Xi = std::move(local_sorted_data);
}

#endif // A2_HPP