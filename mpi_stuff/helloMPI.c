#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> // Include the MPI library header
#include <math.h>
#include <time.h>

void samp_rand(const double a, const double b, const int dim, double *x);
double f(double *x);

int main(int argc, char **argv)
{
    int rank, size;
    MPI_Init(&argc, &argv); // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the process ID (rank)
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get the total number of processes

    // Check for input argument: N samples per process
    if (argc < 2) {
        if (rank == 0) {
            printf("Usage: %s N_samples_per_process\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    int N = atoi(argv[1]);

    srand(time(NULL) * (rank + 1));  // Unique random seed per process

    const int dim = 2;
    double x[dim];
    double local_max = -1;

    for (int i = 1; i <= N; ++i) {
        samp_rand(-1.0, 1.0, dim, x);
        double f_val = f(x);

        // Print details for this sample
        // printf("Process %d: sample %d of %d, f(x1=%.3f, x2=%.3f) = %.5e\n",
        //        rank, i, N, x[0], x[1], f_val);

        if (f_val > local_max) {
            local_max = f_val;
        }
    }

    printf("Hello world from process %d of %d = %.5e\n", rank, size, local_max);
    MPI_Finalize(); // Gracefully shutdown MPI
    return 0;
}

void samp_rand(const double a, const double b, const int dim, double *x) {
    for (int i = 0; i < dim; ++i) {
        double tmp = ((double) rand()) / ((double) RAND_MAX);
        x[i] = (b - a) * tmp + a;
    }
}

double f(double *x) {
    double x1 = x[0];
    double x2 = x[1];
    return exp(-(1.0 - x1)* (1.0 - x1) - 100 * (x2 - x1 * x1) * (x2 - x1 * x1));
}
