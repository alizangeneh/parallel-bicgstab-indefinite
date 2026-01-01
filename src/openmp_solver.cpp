#include "../include/sparse_matrix.h"

#include <vector>
#include <iostream>
#include <chrono>

int main(int argc, char** argv)
{
    if (argc < 2) return -1;

    CSRMatrix A;
    if (!load_matrix_market(argv[1], A)) return -2;

    std::size_t n = A.nrows;
    std::vector<double> x(n, 0.0), b(n, 1.0);
    std::vector<double> r(n), r0(n), p(n), v(n), s(n), t(n);

    spmv_openmp(A, x, r);
    #pragma omp parallel for
    for (std::size_t i = 0; i < n; ++i)
        r[i] = b[i] - r[i];

    r0 = r;

    double rho = 1, alpha = 1, omega = 1;
    double tol = 1e-8;
    int max_iter = 5000, iter = 0;

    auto start = std::chrono::high_resolution_clock::now();

    while (iter < max_iter && norm2(r) > tol) {
        double rho_new = dot(r0, r);
        double beta = (rho_new / rho) * (alpha / omega);
        rho = rho_new;

        #pragma omp parallel for
        for (std::size_t i = 0; i < n; ++i)
            p[i] = r[i] + beta * (p[i] - omega * v[i]);

        spmv_openmp(A, p, v);
        alpha = rho / dot(r0, v);

        #pragma omp parallel for
        for (std::size_t i = 0; i < n; ++i)
            s[i] = r[i] - alpha * v[i];

        spmv_openmp(A, s, t);
        omega = dot(t, s) / dot(t, t);

        #pragma omp parallel for
        for (std::size_t i = 0; i < n; ++i)
            x[i] += alpha * p[i] + omega * s[i];

        #pragma omp parallel for
        for (std::size_t i = 0; i < n; ++i)
            r[i] = s[i] - omega * t[i];

        ++iter;
    }

    auto end = std::chrono::high_resolution_clock::now();
    double time =
        std::chrono::duration<double>(end - start).count();

    std::cout << iter << "," << time << "," << norm2(r) << std::endl;
    return 0;
}
