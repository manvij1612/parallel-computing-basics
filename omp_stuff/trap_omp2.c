#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
int main(int argc, char **argv) {
    int N = atoi( argv[1] ); // convert command-line input to N = number of points
    //printf("N = %i\n",N);
    
    double dx = 2./(N-1); // \delta x spacing when interval is [-1,1]
    double integral = 0.0;
    omp_set_num_threads(4);
    // add in contribution at x=-1
    double x = -1.0;
    double f = 1. / (1. + x*x);
    integral = integral + 0.5*dx*f;
    // add in contribution at x=1
    x = 1.0;
    f = 1. / (1. + x*x);
    integral = integral + 0.5*dx*f;
    double integral_thread[4] = {0}; // initialize all to zero
    #pragma omp parallel private(x,f)
    {
        int id = omp_get_thread_num();
        #pragma omp for
        for (int i=1;i<N-1;++i) {
            x = -1. + dx*i; // compute x_i
            f = 1. / (1. + x*x); // compute f_i = f(x_i)
            integral_thread[id] = integral_thread[id] + dx*f;
        }
    }
    // each thread will contribute its portion to the total integral
    for(int i=0; i< 4; i++) {
        integral = integral + integral_thread[i];
    }
    const double pi = 3.141592653589793;
    double absolute_error = fabs(integral - pi / 2.);
    printf("N = %i, integral = %f, error = %1.2e\n",N,integral,absolute_error);
    return 0;
}
