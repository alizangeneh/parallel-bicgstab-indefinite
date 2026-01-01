@echo off
setlocal EnableExtensions EnableDelayedExpansion

REM =====================================================
REM Benchmark Runner for Parallel Iterative Solvers
REM Runs serial and OpenMP solvers with multiple threads
REM Logs results to benchmarks/results/benchmark_results.csv
REM =====================================================

REM ---- Paths ----
set ROOT_DIR=..\..
set BIN_DIR=%ROOT_DIR%\bin
set RESULTS_DIR=%ROOT_DIR%\benchmarks\results
set RESULTS_FILE=%RESULTS_DIR%\benchmark_results.csv
set MATRICES_DIR=%ROOT_DIR%\benchmarks\test_matrices

REM ---- Executables ----
set SERIAL_EXE=%BIN_DIR%\serial_solver.exe
set OMP_EXE=%BIN_DIR%\openmp_solver.exe

REM ---- Benchmark parameters ----
set MATRIX=bcsstk18.mtx
set THREAD_LIST=1 2 4 8 16

REM ---- Ensure results directory exists ----
if not exist "%RESULTS_DIR%" (
    mkdir "%RESULTS_DIR%"
)

REM ---- CSV header (only if file does not exist) ----
if not exist "%RESULTS_FILE%" (
    echo matrix_name,solver,parallel_model,threads,iterations,final_residual,time_seconds,speedup,efficiency > "%RESULTS_FILE%"
)

echo.
echo ==============================================
echo Running benchmarks for matrix: %MATRIX%
echo ==============================================
echo.

REM =====================================================
REM 1) SERIAL RUN
REM =====================================================
echo Running SERIAL solver...

REM Expected output format from solver:
REM iterations,time_seconds,final_residual

for /f "tokens=1,2,3 delims=," %%a in ('
    "%SERIAL_EXE%" "%MATRICES_DIR%\%MATRIX%"
') do (
    set SERIAL_ITER=%%a
    set SERIAL_TIME=%%b
    set SERIAL_RES=%%c
)

echo %MATRIX%,serial,none,1,%SERIAL_ITER%,%SERIAL_RES%,%SERIAL_TIME%,1.00,1.00 >> "%RESULTS_FILE%"

REM =====================================================
REM 2) PARALLEL RUNS (OpenMP)
REM =====================================================
for %%t in (%THREAD_LIST%) do (

    if %%t==1 (
        REM Skip duplicate OpenMP=1 run (already have serial baseline)
        goto :continue
    )

    echo Running OpenMP solver with %%t threads...

    set OMP_NUM_THREADS=%%t

    for /f "tokens=1,2,3 delims=," %%a in ('
        "%OMP_EXE%" "%MATRICES_DIR%\%MATRIX%"
    ') do (
        set OMP_ITER=%%a
        set OMP_TIME=%%b
        set OMP_RES=%%c
    )

    REM ---- Compute speedup and efficiency ----
    set SPEEDUP=
    set EFFICIENCY=

    for /f %%s in ('powershell -command "[math]::Round(%SERIAL_TIME% / %OMP_TIME%, 2)"') do set SPEEDUP=%%s
    for /f %%e in ('powershell -command "[math]::Round(%SPEEDUP% / %%t, 2)"') do set EFFICIENCY=%%e

    echo %MATRIX%,parallel,openmp,%%t,%OMP_ITER%,%OMP_RES%,%OMP_TIME%,%SPEEDUP%,%EFFICIENCY% >> "%RESULTS_FILE%"

    :continue
)

echo.
echo ==============================================
echo Benchmarking completed.
echo Results saved to:
echo %RESULTS_FILE%
echo ==============================================
echo.

endlocal
exit /b 0
