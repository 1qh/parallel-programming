#include <stdio.h>
#include <omp.h>

static long num_steps = 1000000000;
double step;

#define NUM_THREADS 8

void main()
{
    double runtime = omp_get_wtime();
    
    int i;
    double pi, sum = 0;

    step = 1 / (double)num_steps;
    
    #pragma omp parallel
    {
        double x;
        #pragma omp for reduction(+:sum)
        for (i = 0; i < num_steps; i++) {
            x = (i + 0.5) * step;
            sum += 4 / (1 + x * x);
        }
    }
    pi = step * sum;

    printf("%f\n", pi);

    printf("%f ms\n", 1000 * (omp_get_wtime() - runtime));
}