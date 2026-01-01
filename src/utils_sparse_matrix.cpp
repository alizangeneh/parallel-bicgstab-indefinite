#include "../include/sparse_matrix.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

#ifdef _OPENMP
#include <omp.h>
#endif

bool load_matrix_market(const std::string& filename, CSRMatrix& A)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) return false;

    std::string line;
    do {
        std::getline(fin, line);
    } while (line[0] == '%');

    std::istringstream header(line);
    std::int64_t rows, cols, nnz;
    header >> rows >> cols >> nnz;

    A = CSRMatrix(rows, cols, nnz);

    std::vector<std::int64_t> row_count(rows, 0);
    std::vector<std::int64_t> r(nnz), c(nnz);
    std::vector<double> v(nnz);

    for (std::int64_t i = 0; i < nnz; ++i) {
        fin >> r[i] >> c[i] >> v[i];
        --r[i]; --c[i];
        row_count[r[i]]++;
    }

    A.row_ptr[0] = 0;
    for (std::int64_t i = 0; i < rows; ++i)
        A.row_ptr[i + 1] = A.row_ptr[i] + row_count[i];

    std::vector<std::int64_t> offset = A.row_ptr;
    for (std::int64_t i = 0; i < nnz; ++i) {
        std::int64_t row = r[i];
        std::int64_t idx = offset[row]++;
        A.col_idx[idx] = c[i];
        A.values[idx]  = v[i];
    }

    return A.is_valid();
}

void spmv(const CSRMatrix& A,
          const std::vector<double>& x,
          std::vector<double>& y)
{
    for (std::int64_t i = 0; i < A.nrows; ++i) {
        double sum = 0.0;
        for (std::int64_t j = A.row_ptr[i]; j < A.row_ptr[i + 1]; ++j)
            sum += A.values[j] * x[A.col_idx[j]];
        y[i] = sum;
    }
}

void spmv_openmp(const CSRMatrix& A,
                 const std::vector<double>& x,
                 std::vector<double>& y)
{
    #pragma omp parallel for schedule(static)
    for (std::int64_t i = 0; i < A.nrows; ++i) {
        double sum = 0.0;
        for (std::int64_t j = A.row_ptr[i]; j < A.row_ptr[i + 1]; ++j)
            sum += A.values[j] * x[A.col_idx[j]];
        y[i] = sum;
    }
}

double dot(const std::vector<double>& x,
           const std::vector<double>& y)
{
    double result = 0.0;
    #pragma omp parallel for reduction(+:result)
    for (std::size_t i = 0; i < x.size(); ++i)
        result += x[i] * y[i];
    return result;
}

double norm2(const std::vector<double>& x)
{
    return std::sqrt(dot(x, x));
}

void axpy(double a,
          const std::vector<double>& x,
          std::vector<double>& y)
{
    #pragma omp parallel for
    for (std::size_t i = 0; i < x.size(); ++i)
        y[i] += a * x[i];
}

void scal(double a,
          std::vector<double>& x)
{
    #pragma omp parallel for
    for (std::size_t i = 0; i < x.size(); ++i)
        x[i] *= a;
}

void print_matrix_info(const CSRMatrix& A)
{
    std::cout << "Matrix: "
              << A.nrows << " x " << A.ncols
              << ", nnz = " << A.nnz << std::endl;
}
