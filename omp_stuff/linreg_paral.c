#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<omp.h>

double compute_partial_m(double m,  double b, double x[], double y[], int n);
double compute_partial_b(double m,  double b, double x[], double y[], int n);
double compute_mse(double m,  double b, double x[], double y[], int n);
int main(int argc, char **argv){
    double start_time = omp_get_wtime();
    if (argc != 6) {
        printf("Usage: %s <datafile.csv> <m0> <b0> <alpha> <max_iter> \n", argv[0]);
        return 1;
    }
    char *filename = argv[1];
    double m0 = atof(argv[2]);
    double b0 = atof(argv[3]);
    double alpha = atof(argv[4]);
    double max_iter = atoi(argv[5]);


    printf("Data file is: %s\n", argv[1]);

    FILE *fp =fopen(argv[1], "r");
    if (!fp){
        printf(" Error: Could not open file %s\n", argv[1]);
        return 1;
    }
    const int MAX_POINTS = 9855000;
    double *xvals = malloc(MAX_POINTS * sizeof(double));
    double *yvals = malloc(MAX_POINTS * sizeof(double));
    if (!xvals || !yvals) {
    printf("Error: cannot allocate memory.\n");
    return 1;
    }

    int n_points = 0;
    while (!feof(fp)){
        double x, y;
        int matched = fscanf(fp, "%lf,%lf\n", &x, &y);
        if (matched == 2) {
            xvals[n_points] = x;
            yvals[n_points] = y;
            n_points++;
            if (n_points >= MAX_POINTS) {
                printf("Warning : reached max storage limit (%d). Stopped read. \n", MAX_POINTS);
                break;
            }
        } else {
            printf("Warning: skipping line in file\n");
        }
    }
    fclose(fp);
    
    double m = m0;
    double b = b0;
    double partial_m;
    double partial_b;
    double mse;
    // double mse_values[MAX_ITERATIONS], m_values[MAX_ITERATIONS], b_values[MAX_ITERATIONS];

    for (int k = 0; k <= max_iter; k++) {
        partial_m = compute_partial_m(m, b, xvals, yvals, n_points);
        partial_b = compute_partial_b(m, b, xvals, yvals, n_points);

        m -= alpha * partial_m;
        b -= alpha * partial_b;
        mse = compute_mse(m, b, xvals, yvals, n_points);

        printf("epoch = %d, m = %.8f, b = %.8f, MSE = %g \n", k, m, b, mse);

    }
    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;

    printf("OMP wall time: %.6f seconds\n", elapsed_time);
    return 0;
}


double compute_partial_m(double m,  double b, double x[], double y[], int n){
    double partial_m = 0.0;
    #pragma omp parallel for reduction(+:partial_m)
    for(int k = 0; k <n; k++) {
        partial_m += (-2.0) * (x[k] * (y[k] - (m * x[k] + b)));
    }
    return partial_m / n ;
}

double compute_partial_b(double m,  double b, double x[], double y[], int n){
    double partial_b = 0.0;
    #pragma omp parallel for reduction(+:partial_b)
    for(int k = 0; k < n; k++) {
        partial_b += (-2.0) * (y[k] - (m * x[k] + b));
    }
    return partial_b / n;
}

double compute_mse(double m,  double b, double x[], double y[], int n){
    double mse = 0.0;
    #pragma omp parallel for reduction(+:mse)
    for(int k = 0; k < n; k++) {
        double error = (y[k] - (m * x[k] + b));
        mse += error * error;
    }
    return mse / n;
}

