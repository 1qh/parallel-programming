#include <stdio.h>
#include <omp.h>

static long num_steps = 1000000000;
double step;

#define NUM_THREADS 8

void main()
{
    double runtime = omp_get_wtime();
    
    int num_threads;
    double pi = 0;

    step = 1 / (double)num_steps;

    omp_set_num_threads(NUM_THREADS);

    #pragma omp parallel
    {
        int i, id, temp_num_threads;
        double x, sum;
        id = omp_get_thread_num();
        temp_num_threads = omp_get_num_threads();
        
        if (id == 0) num_threads = temp_num_threads;

        for (i = id, sum = 0; i < num_steps; i += temp_num_threads) {
            x = (i + 0.5) * step;
            sum += 4 / (1 + x * x);
        }
        sum *= step;
        #pragma omp atomic
        pi += sum;
    }

    printf("Number of threads: %d\n", num_threads);

    printf("%f\n", pi);

    printf("%f ms\n", 1000 * (omp_get_wtime() - runtime));
}