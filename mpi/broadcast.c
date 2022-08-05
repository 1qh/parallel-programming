#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <mpi.h>

void debug(int rank, char *format, ...) {
    va_list args;
    va_start(args, format);

    printf("%d - ", rank);
    vprintf(format, args);

    va_end(args);
}
int broadcast(int rank, int num_procs){
    long val;
    int broadcaster_rank = num_procs - 1;

    if (rank == broadcaster_rank) {
        srandom(time(NULL) + rank);
        val = random() % 100;
        debug(rank, "broadcast %ld\n", val);
    }
    MPI_Bcast(
        (void *)&val,
        1,
        MPI_LONG,
        broadcaster_rank,
        MPI_COMM_WORLD
    );

    if (rank != broadcaster_rank) {
        debug(rank, "received %ld\n", val);
    }
    return val;
}
void barrier(int rank, long val){

    double nap_time = 1.2 * rank;
    sleep(nap_time);

    debug(rank, "sleep %1.1fs\n", nap_time);
    debug(rank, "enter BARRIER\n");

    MPI_Barrier(MPI_COMM_WORLD);

    debug(rank, "leave\n");
}

int main(int argc, char *argv[]){

    int num_procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    debug(rank, "hello (p=%d)\n", num_procs);
    
    long val = broadcast(rank, num_procs);
    
    barrier(rank, val);
    
    debug(rank, "Bye\n");
    
    MPI_Finalize();
    return 0;
}