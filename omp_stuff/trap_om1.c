#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
int main(int argc, char **argv) {
    int N = atoi( argv[1] ); // convert command-line input to N = number of points
    //printf("N = %i\n",N);
    omp_set_num_threads(4);
    double dx = 2./(N-1); // \delta x spacing when interval is [-1,1]
    double integral = 0.0;
    // add in contribution at x=-1
    double x = -1.0;
    double f = 1. / (1. + x*x);
    integral = integral + 0.5*dx*f;
    // add in contribution at x=1
    x = 1.0;
    f = 1. / (1. + x*x);
    integral = integral + 0.5*dx*f;

    #pragma omp parallel for private(x, f)
    for (int i=1;i<N-1;++i) {
        x = -1. + dx*i; // compute x_i
        f = 1. / (1. + x*x); // compute f_i = f(x_i)
        
        #pragma omp atomic
        integral = integral + dx*f;
    }
    
    const double pi = 3.141592653589793;
    double absolute_error = fabs(integral - pi / 2.);
    printf("N = %i, integral = %f, error = %1.2e\n",N,integral,absolute_error);
    return 0;
}
