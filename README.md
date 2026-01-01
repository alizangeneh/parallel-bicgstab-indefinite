Parallel BiCGStab Solver for Sparse Indefinite Linear Systems

This repository presents a research-oriented implementation of serial and OpenMP-parallel BiCGStab solvers for large sparse indefinite linear systems. The project focuses on efficient sparse matrix-vector multiplication, strong scaling behavior, and reproducible performance benchmarking in shared-memory environments.

Motivation

Large sparse linear systems frequently arise in scientific computing applications such as computational physics, PDE discretizations, and numerical simulations. For indefinite or non-symmetric systems, iterative Krylov subspace methods are often preferred over direct solvers due to their lower memory requirements and better scalability.

This project aims to provide a clean, minimal, and extensible implementation of the BiCGStab method, serving as a foundation for high-performance iterative solvers.

Features

Serial baseline implementation of the BiCGStab algorithm
OpenMP-parallel solver with row-wise parallelization
Compressed Sparse Row (CSR) matrix representation
Support for Matrix Market (.mtx) input format
Automated benchmarking and CSV-based result logging
Strong scaling and parallel efficiency analysis

Project Structure

parallel-bicgstab-indefinite/
include/ Sparse matrix and linear algebra interfaces
src/ Serial and OpenMP solver implementations
benchmarks/ Benchmark scripts, results, and test matrices
report/ Technical report and documentation
plots/ Generated figures

Implementation Overview

Sparse Matrix Storage: Compressed Sparse Row (CSR) format
Solver: BiConjugate Gradient Stabilized (BiCGStab)
Parallelization: OpenMP shared-memory parallelism
Dominant Kernel: Sparse matrix-vector multiplication (SpMV)

Benchmarking

Benchmarks are executed using real-world sparse matrices. For each run, the number of iterations, execution time, final residual norm, speedup, and parallel efficiency are recorded. Results are stored in CSV format to ensure reproducibility and ease of analysis.

Build and Run (Example)

This project can be built with any C++ compiler supporting OpenMP.

Example using GCC:
g++ -O3 -fopenmp src/*.cpp -Iinclude -o openmp_solver

Future Extensions

Preconditioning techniques such as Jacobi or ILU
Hybrid MPI and OpenMP parallelization
GPU acceleration for sparse matrix-vector multiplication
Distributed-memory scalability studies

Author

Ali Zangeneh
Background in Computer Engineering with a focus on high-performance computing, numerical methods, and parallel programming.

License

This project is intended for academic and research purposes.
