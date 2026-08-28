# Parallel Computing Exercises

Small C programs exploring shared-memory (**OpenMP**) and distributed-memory
(**MPI**) parallelism, built while learning HPC fundamentals.

## Build

```bash
make            # build everything
make omp        # OpenMP examples only
make mpi        # MPI examples only
```

Requires a C compiler with OpenMP support and an MPI implementation
(`brew install libomp open-mpi` on macOS; see the `Makefile` header for
Apple-clang flags).

## OpenMP examples (`omp_stuff/`)

| File | What it shows |
| --- | --- |
| `parfor.c` | Canonical `#pragma omp parallel for` — parallel element-wise `sqrt` |
| `trap_om1.c` | Trapezoidal integration of `1/(1+x²)` with `#pragma omp atomic` on the accumulator — **correct but slow** (serialised updates) |
| `trap_omp2.c` | Same integral with a per-thread accumulator array reduced serially afterwards — faster, but suffers **false sharing** on the padded array |
| `trap_omp3.c` | Same integral with `reduction(+:integral)` — **the idiomatic, fastest version** |
| `linreg_paral.c` | Batch gradient descent for 1-D linear regression; the gradient and MSE loops use `reduction` |

`trap_om1 → trap_omp2 → trap_omp3` is a deliberate progression from a naive
parallelisation to the idiomatic one.

Run e.g.:

```bash
./omp_stuff/trap_omp3 1000000
./omp_stuff/linreg_paral data.csv 0.0 0.0 0.01 1000
```

## MPI examples (`mpi_stuff/`)

| File | What it shows |
| --- | --- |
| `helloMPI.c` | Ranks, unique per-rank RNG seeding, per-rank Monte-Carlo maximum of a Rosenbrock-type function |
| `myCode.c` | Monte-Carlo **global minimisation** of an external `secret_function` with `MPI_Reduce(MPI_MIN)`; times the reduction separately from the whole run |

Run e.g.:

```bash
mpirun -np 4 ./mpi_stuff/helloMPI 100000
```

> **Note:** `myCode.c` depends on `secret_function.h` and a `secret_function`
> implementation that are not included here, so it is excluded from the default
> `make` target. Drop in your own objective function to build it.

## To do

- Add a speedup-vs-thread-count benchmark and plot for the trapezoid example.

## License

MIT — see [LICENSE](LICENSE).
