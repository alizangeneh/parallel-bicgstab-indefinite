# Benchmark Results

This directory contains raw benchmark results for the parallel iterative solvers.

## File Description

- `benchmark_results.csv`  
  Stores execution results for serial and OpenMP-parallel solvers.

## Columns Description

- **matrix_name**: Name of the sparse matrix used in the test
- **solver**: Serial or parallel implementation
- **parallel_model**: Parallel backend (OpenMP or none)
- **threads**: Number of threads used
- **iterations**: Number of solver iterations until convergence
- **final_residual**: Final residual norm ||Ax - b||
- **time_seconds**: Total execution time in seconds
- **speedup**: Speedup relative to serial execution
- **efficiency**: Parallel efficiency (speedup / threads)

## Notes

- All experiments are run on the same hardware.
- Compiler optimization flags are fixed across runs.
- Each benchmark is executed multiple times and averaged when needed.
