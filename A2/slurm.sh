#!/bin/bash

####### specify cluster configuration
#SBATCH --cluster=ub-hpc
#SBATCH --partition=general-compute
#SBATCH --qos=general-compute

####### select resources (here we specify required memory)
#SBATCH --mem=128000
#SBATCH --constraint="CPU-Gold-6230&CASCADE-LAKE-IB"

####### make sure no other jobs are assigned to your nodes
#SBATCH --exclusive

####### further customizations
### name of your job
#SBATCH --job-name="isort"

### files to store stdout and stderr (%j will be replaced by the job id)
#SBATCH --output=%j.stdout
#SBATCH --error=%j.stderr

#SBATCH --nodes=8

### how many cores to use on each node
#SBATCH --ntasks-per-node=32

### max time the job will run
#SBATCH --time=05:00:00

module load gcc openmpi/4.1.1
# export I_MPI_PMI_LIBRARY=/usr/lib64/libpmi.so

echo "JOB_ID=$SLURM_JOB_ID"
echo "hostname=$(hostname)"
echo "ARCH=$CCR_ARCH"
date -u

lscpu

echo "-N 8 --ntasks-per-node=1 Running n=1000000000"
srun -N 8 --ntasks-per-node=1 ./a2 10000000000

echo "-N 8 --ntasks-per-node=2 Running n=20000000000"
srun -N 8 --ntasks-per-node=2 ./a2 20000000000

echo "-N 8 --ntasks-per-node=4 Running n=40000000000"
srun -N 8 --ntasks-per-node=4 ./a2 40000000000

echo "-N 8 --ntasks-per-node=8 Running n=80000000000"
srun -N 8 --ntasks-per-node=8 ./a2 80000000000

echo "-N 8 --ntasks-per-node=16 Running n=160000000000"
srun -N 8 --ntasks-per-node=16 ./a2 160000000000
