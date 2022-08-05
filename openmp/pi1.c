#include <stdio.h>
#include <omp.h>

static long num_steps = 1000000000;
double step;

#define NUM_THREADS 8
#define PAD 8

void main()
{
    double runtime = omp_get_wtime();
    
    int i, num_threads;
    double sum[NUM_THREADS][PAD];
    double pi = 0;

    step = 1 / (double)num_steps;

    omp_set_num_threads(NUM_THREADS);

    #pragma omp parallel
    {
        int i, id, temp_num_threads;
        double x;
        id = omp_get_thread_num();
        temp_num_threads = omp_get_num_threads();
        
        if (id == 0) num_threads = temp_num_threads;

        for (i = id, sum[id][0] = 0; i < num_steps; i += temp_num_threads)
        {
            x = (i + 0.5) * step;
            sum[id][0] += 4 / (1 + x * x);
        }
    }

    printf("Number of threads: %d\n", num_threads);

    for (i = 0; i < num_threads; i++) 
        pi += sum[i][0] * step;

    printf("%f\n", pi);

    printf("%f ms\n", 1000 * (omp_get_wtime() - runtime));
}