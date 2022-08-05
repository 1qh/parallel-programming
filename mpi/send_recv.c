#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void round_robin(int rank, int procs){
    long int mine, val_prev;
    int rank_next = (rank + 1) % procs;
    int rank_prev = rank == 0 ? procs - 1 : rank - 1;

    MPI_Status status;

    srandom(time(NULL) + rank);

    mine = random() % 100;

    if (rank % 2 == 0) {

        printf("%d: send %ld to %d\n", rank, mine, rank_next);
        MPI_Send(
            (void *)&mine,
            1,
            MPI_LONG,
            rank_next,
            1,
            MPI_COMM_WORLD
        );
        
        printf("%d: receive from %d number ", rank, rank_prev);
        MPI_Recv(
            (void *)&val_prev,
            1,
            MPI_LONG,
            rank_prev,
            1,
            MPI_COMM_WORLD,
            &status
        );
        printf("%ld\n", val_prev);

    } else {

        printf("%d: receive from %d number ", rank, rank_prev);
        MPI_Recv(
            (void *)&val_prev,
            1,
            MPI_LONG,
            rank_prev,
            1,
            MPI_COMM_WORLD,
            &status
        );
        printf("%ld\n", val_prev);

        printf("%d: send %ld to %d\n", rank, mine, rank_next);
        MPI_Send(
            (void *)&mine,
            1,
            MPI_LONG,
            rank_next,
            1,
            MPI_COMM_WORLD
        );
    }
}

int main(int argc, char *argv[]){

    int num_procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("%d: hello (p=%d)\n", rank, num_procs);

    round_robin(rank, num_procs);

    printf("%d: goodbye\n", rank);    

    MPI_Finalize();
    return 0;
}