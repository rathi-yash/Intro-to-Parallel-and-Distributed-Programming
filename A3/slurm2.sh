#!/bin/bash

####### specify cluster configuration
#SBATCH --partition=general-compute
#SBATCH --qos=general-compute

####### select resources (here we specify required memory and GPU type)
#SBATCH --mem=32G
#SBATCH --constraint=AVX512&CPU-Gold-6130&V100
#SBATCH --exclusive


### name of your job
#SBATCH --job-name="2gaussian_kde_run"

### files to store stdout and stderr (%j will be replaced by the job id)
#SBATCH --output=./logs/%j.stdout
#SBATCH --error=./logs/%j.stderr

### how many nodes to allocate for the job
### for GPU jobs, typically we use only one node
#SBATCH --nodes=1

### how many cores to use on each node
### Adjust based on your program's CPU requirements
#SBATCH --ntasks-per-node=8

### max time the job will run
#SBATCH --time=01:00:00

####### run commands
echo "JOB_ID=$SLURM_JOB_ID"
echo "hostname=`hostname`"
echo "ARCH=$CCR_ARCH"
date -u
nvidia-smi

# print gpu info
echo "GPU INFO:"
nvidia-smi --query-gpu=gpu_name,driver_version,memory.total --format=csv

printf "\n"
lscpu

module load cuda

# use gpu 0
export CUDA_VISIBLE_DEVICES=0

# Run CUDA program
echo "Running on GPU with n=1000000, h=0.1"
./a3 1000000 0.1
echo "Running on GPU with n=2000000, h=0.1"
./a3 2000000 0.1

echo "Running on CPU with n=1000000, h=0.1"
./a3_cpu 100000 0.1
echo "Running on GPU with n=100000, h=0.1"
./a3 100000 0.1
echo "Running on CPU with n=200000, h=0.1"
./a3_cpu 200000 0.1
echo "Running on GPU with n=200000, h=0.1"
./a3 200000 0.1
echo "Running on CPU with n=300000, h=0.1"
./a3_cpu 300000 0.1
echo "Running on GPU with n=300000, h=0.1"
./a3 300000 0.1
echo "Running on CPU with n=400000, h=0.1"
./a3_cpu 400000 0.1
echo "Running on GPU with n=400000, h=0.1"
./a3 400000 0.1
echo "Running on CPU with n=500000, h=0.1"
./a3_cpu 500000 0.1
echo "Running on GPU with n=500000, h=0.1"
./a3 500000 0.1
