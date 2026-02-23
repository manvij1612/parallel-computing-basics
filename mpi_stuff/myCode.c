#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "secret_function.h"
#include <math.h>
#include <time.h>
#include<omp.h>

void samp_rand(const double a, const double b, const int dim, double *x);

int main(int argc, char **argv)
{   
    double start_time = omp_get_wtime();
    int rank, size;
    MPI_Init(&argc, &argv); // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the process ID (rank)
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get the total number of processes
    long N = atol(argv[1]);
    srand(time(NULL) * rank);  // Unique random seed per process
    const int dim = 2;
    double x[dim];
    double local_min = 50000;
    for (long int i = 1; i <= N; ++i) {
        samp_rand(-512.0, 512.0, dim, x);
        double f_val = secret_function(x[0], x[1]);
        if (f_val < local_min) {
            local_min = f_val;          
        }
        printf("Process %d, sample %ld with f_i = %e\n",rank, i, f_val);
    }
    //  printf("Process %d local minimum = %e\n", rank, local_min);
    double start_time1 = omp_get_wtime();
    double process_local_min = local_min;
    double process_global_min=50000;
    MPI_Reduce(&process_local_min,&process_global_min,1,MPI_DOUBLE,MPI_MIN,0,MPI_COMM_WORLD);
    if (rank == 0) {
        printf(" Global minimum found: %f \n", process_global_min);
    }
    MPI_Finalize();
    double end_time = omp_get_wtime();
    double elapsed_time1 = end_time - start_time1;
    double elapsed_time = end_time - start_time;
    if (rank == 0) {
        
        printf("OMP wall time for reduction: %.6f seconds\n", elapsed_time1);
        printf("OMP wall time for entire code: %.6f seconds\n", elapsed_time);
    }
    return 0;
}
void samp_rand(const double a, const double b, const int dim, double *x) {
    for (int i = 0; i < dim; ++i) {
        double tmp = ((double) rand()) / ((double) RAND_MAX);
        x[i] = (b - a) * tmp + a;
    }
}

