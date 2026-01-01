#pragma once

#include <vector>
#include <cstdint>
#include <string>

/*
 * Sparse matrix representation using Compressed Sparse Row (CSR) format.
 *
 * This structure is designed for:
 *  - Iterative solvers (BiCGStab, GMRES, CG)
 *  - Efficient sparse matrix-vector multiplication (SpMV)
 *  - Serial and OpenMP-parallel execution
 *
 * Indices are 0-based.
 */

struct CSRMatrix
{
    // Matrix dimensions
    std::int64_t nrows = 0;
    std::int64_t ncols = 0;
    std::int64_t nnz   = 0;   // number of non-zero entries

    // CSR storage
    std::vector<std::int64_t> row_ptr;  // size: nrows + 1
    std::vector<std::int64_t> col_idx;  // size: nnz
    std::vector<double>       values;   // size: nnz

    // Constructors
    CSRMatrix() = default;

    CSRMatrix(std::int64_t rows, std::int64_t cols, std::int64_t nonzeros)
        : nrows(rows), ncols(cols), nnz(nonzeros),
          row_ptr(rows + 1), col_idx(nonzeros), values(nonzeros)
    {}

    // Clear all data
    void clear()
    {
        nrows = ncols = nnz = 0;
        row_ptr.clear();
        col_idx.clear();
        values.clear();
    }

    // Basic consistency check
    bool is_valid() const
    {
        return (row_ptr.size() == static_cast<std::size_t>(nrows + 1)) &&
               (col_idx.size() == static_cast<std::size_t>(nnz)) &&
               (values.size()  == static_cast<std::size_t>(nnz));
    }
};

/*
 * ===========================
 * Sparse Matrix I/O
 * ===========================
 */

/*
 * Load a sparse matrix from a Matrix Market (.mtx) file.
 * Assumes:
 *  - Real-valued matrix
 *  - Coordinate format
 *
 * The matrix is converted internally to CSR format.
 *
 * Returns:
 *  - true on success
 *  - false on failure
 */
bool load_matrix_market(const std::string& filename, CSRMatrix& A);

/*
 * ===========================
 * Sparse Linear Algebra Kernels
 * ===========================
 */

/*
 * Sparse Matrix-Vector multiplication:
 *   y = A * x
 *
 * Requirements:
 *  - x.size() == A.ncols
 *  - y.size() == A.nrows
 */
void spmv(const CSRMatrix& A,
          const std::vector<double>& x,
          std::vector<double>& y);

/*
 * Parallel Sparse Matrix-Vector multiplication (OpenMP).
 * The parallelization strategy is row-wise.
 */
void spmv_openmp(const CSRMatrix& A,
                 const std::vector<double>& x,
                 std::vector<double>& y);

/*
 * ===========================
 * Vector Utilities
 * ===========================
 */

/* Compute dot product <x, y> */
double dot(const std::vector<double>& x,
           const std::vector<double>& y);

/* Compute Euclidean norm ||x||_2 */
double norm2(const std::vector<double>& x);

/* y = a * x + y */
void axpy(double a,
          const std::vector<double>& x,
          std::vector<double>& y);

/* x = a * x */
void scal(double a,
          std::vector<double>& x);

/*
 * ===========================
 * Diagnostics
 * ===========================
 */

/* Print basic matrix statistics (rows, cols, nnz) */
void print_matrix_info(const CSRMatrix& A);

