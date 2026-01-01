# Technical Report  
## Parallel BiCGStab Solver for Large Sparse Indefinite Linear Systems

---

## 1. Introduction

Large sparse linear systems arise in a wide range of scientific and engineering applications, including computational physics, plasma simulations, and discretized partial differential equations. When the system matrix is sparse and indefinite, direct solvers often become impractical due to memory and computational constraints.

Iterative Krylov subspace methods provide an efficient alternative, particularly when combined with parallel computing techniques. This report presents the design, implementation, and performance evaluation of a serial and OpenMP-parallel iterative solver for large sparse indefinite linear systems, with a focus on scalability and numerical behavior.

---

## 2. Problem Formulation

We consider linear systems of the form:

\[
A x = b,
\]

where \( A \in \mathbb{R}^{n \times n} \) is a large, sparse, and potentially indefinite matrix, and \( b \in \mathbb{R}^n \) is a given right-hand-side vector.

The sparsity pattern of \( A \) is assumed to be irregular, making dense linear algebra approaches infeasible. The goal is to compute an approximate solution \( x \) such that the residual norm

\[
\| r_k \|_2 = \| b - A x_k \|_2
\]

is sufficiently small.

---

## 3. Numerical Background

### 3.1 Iterative Methods for Indefinite Systems

For symmetric positive definite matrices, the Conjugate Gradient method is commonly used. However, for indefinite or non-symmetric systems, alternative Krylov methods are required.

In this work, the BiConjugate Gradient Stabilized (BiCGStab) method is employed due to its robustness and effectiveness for large sparse indefinite systems. BiCGStab combines the fast convergence properties of BiCG with a stabilization mechanism that reduces irregular convergence behavior.

### 3.2 Sparse Matrix Representation

All matrices are stored using the Compressed Sparse Row (CSR) format. This representation provides efficient access for sparse matrix-vector multiplication (SpMV), which is the dominant computational kernel in Krylov solvers.

---

## 4. Implementation Details

### 4.1 Data Structures

The sparse matrix is represented using CSR format with zero-based indexing. Vectors are stored in contiguous memory layouts to enable efficient memory access and parallelization.

### 4.2 Serial Solver

The serial implementation follows the standard BiCGStab algorithm. At each iteration, the following operations are performed:

- Sparse matrix-vector multiplication
- Vector dot products
- Vector updates (AXPY operations)
- Residual norm computation

The serial solver serves as the performance baseline for all parallel experiments.

### 4.3 OpenMP Parallelization

Parallelization is achieved using OpenMP with a row-wise decomposition strategy. The SpMV kernel is parallelized using OpenMP `parallel for` directives, as it constitutes the main computational bottleneck.

Care is taken to avoid race conditions and to ensure reproducibility across different thread counts. Reduction operations are used where necessary for dot products.

---

## 5. Experimental Setup

### 5.1 Test Matrices

Benchmark matrices are selected from publicly available sparse matrix collections, such as the SuiteSparse Matrix Collection. The selected matrices are large, sparse, and indefinite, representing realistic workloads encountered in scientific computing.

### 5.2 Hardware and Software Environment

All experiments are conducted on a shared-memory multi-core system. The following configuration is used throughout the experiments:

- CPU: Multi-core x86 processor
- Memory: Sufficient RAM to hold all matrices in memory
- Compiler: C++ compiler with OpenMP support
- Optimization flags: `-O3`

### 5.3 Benchmark Methodology

Each experiment is executed using different numbers of OpenMP threads. Execution time, iteration counts, and final residual norms are recorded. Speedup and parallel efficiency are computed relative to the serial baseline.

---

## 6. Results and Performance Analysis

### 6.1 Convergence Behavior

The BiCGStab solver converges reliably for all tested matrices. The number of iterations remains consistent across different thread counts, indicating that parallelization does not affect numerical stability.

### 6.2 Strong Scaling

Strong scaling experiments demonstrate a clear reduction in execution time as the number of threads increases. The observed speedup is close to linear for small thread counts and gradually saturates due to memory bandwidth limitations.

### 6.3 Parallel Efficiency

Parallel efficiency decreases as the number of threads increases, which is expected due to synchronization overheads and shared memory contention. Nevertheless, the solver maintains reasonable efficiency for moderate thread counts.

---

## 7. Discussion

The results confirm that sparse matrix-vector multiplication dominates the overall runtime. While OpenMP parallelization significantly accelerates this kernel, memory access patterns and bandwidth constraints limit scalability.

The current implementation does not employ preconditioning, which may affect convergence speed for more challenging systems. Introducing suitable preconditioners is expected to reduce iteration counts and improve overall performance.

---

## 8. Limitations and Future Work

Several extensions can further improve the solver:

- Integration of preconditioning techniques (e.g., Jacobi, ILU)
- Hybrid parallelization using MPI + OpenMP
- GPU acceleration for SpMV
- Evaluation on larger-scale systems and distributed-memory platforms

These directions provide a clear path toward a full-scale high-performance solver suitable for large scientific applications.

---

## 9. Conclusion

This report presents a serial and OpenMP-parallel implementation of a BiCGStab solver for large sparse indefinite linear systems. The results demonstrate that parallelization effectively reduces execution time while preserving numerical stability.

The project serves as a foundation for further research in high-performance iterative solvers and illustrates the practical challenges and trade-offs involved in parallel sparse linear algebra.
