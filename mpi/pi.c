#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char *argv[]){
    int  n = 1000000, done = 0, rank, numprocs, i;
    double x, pi_split, pi, step, sum = 0;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    step = 1 / (double) n;

    while (!done)
    {
        MPI_Bcast(
            &n,
            1,
            MPI_INT,
            0,
            MPI_COMM_WORLD
        );

        for (i = rank; i < n; i += numprocs) {
            x = step * ((double)i - 0.5);
            sum += 4 / (1 + x * x);
        
        pi_split = step * sum;
    
        MPI_Reduce(
            &pi_split,
            &pi,
            1,
            MPI_DOUBLE,
            MPI_SUM,
            0,
            MPI_COMM_WORLD
        );

        if (rank == 0)
            printf("%.16f\n", pi);
            done = 1;
        }
    }
    MPI_Finalize();
    return 0;
}
