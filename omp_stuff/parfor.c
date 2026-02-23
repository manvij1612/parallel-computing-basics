#include <omp.h>
#include <stdio.h>
#include <math.h>
int main() {
int N = 128;
float a[N];
float b[N];
// serial portion of the code -- fillup b with integers
for (int j=0; j<N; j++) {
b[j] = j+1;
}
// parallel portion of the code
#pragma omp parallel
{
// compute square roots in parallel
#pragma omp for
for(int i=0; i<N; i++){
printf("Thread %d working on component %d\n",omp_get_thread_num(),i);
a[i] = sqrt(b[i]);
}
}
// serial portion of the code -- print results
for (int j=0; j<N; j++) {
printf("square root of %f is = %f\n",b[j],a[j]);
}
return 0;
}
