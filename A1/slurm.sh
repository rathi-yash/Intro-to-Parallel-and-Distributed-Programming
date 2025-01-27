#!/bin/bash

#SBATCH --cluster=ub-hpc
#SBATCH --partition=general-compute
#SBATCH --qos=general-compute
#SBATCH --mem=256000
#SBATCH --exclusive
#SBATCH --job-name="yrathi2_A1Q2"
#SBATCH --output=./%j.stdout
#SBATCH --error=./%j.stderr
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=63
#SBATCH --time=05:00:00

echo "JOB_ID=$SLURM_JOB_ID"
echo "hostname=`hostname`"
echo "ARCH=$CCR_ARCH"
date -u
echo ""

lscpu
echo ""

# List of thread counts to test
threads=(1 2 4 8 16 32)

# Loop through each thread count
for t in "${threads[@]}"
do
    echo "Testing with $t threads"
    
    # Strong scaling test
    echo "Strong scaling test: N=10000, M=10000 OMP_NUM_THREADS=$t"
    OMP_NUM_THREADS=$t ./a1 10000 10000
    OMP_NUM_THREADS=$t ./a1 10000 10000
    OMP_NUM_THREADS=$t ./a1 10000 10000
    
    # Weak scaling test
    N=$((10000 * t))
    echo "Weak scaling test: N=$N, M=10000 OMP_NUM_THREADS=$t"
    OMP_NUM_THREADS=$t ./a1 $N 10000
    OMP_NUM_THREADS=$t ./a1 $N 10000
    OMP_NUM_THREADS=$t ./a1 $N 10000
    
    echo ""
done