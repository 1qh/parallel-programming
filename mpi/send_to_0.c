#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]){

    const int tag = 42;

    int rank, size, source, dest, err, i;

    MPI_Status status;

    int msg[2];

    err = MPI_Init(&argc, &argv);

    if (err != MPI_SUCCESS) {
        printf("failed\n");
        exit(1);
    }

    err = MPI_Comm_size(MPI_COMM_WORLD, &size);
    err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size < 2) {
        printf("use at least 2 procs\n");
        MPI_Finalize();
        exit(0);
    }
// Process 0 - receiver
    if (rank == 0) {
        for (i = 1; i < size; i++) {
            err = MPI_Recv(
                msg,
                2,
                MPI_INT,
                MPI_ANY_SOURCE,
                tag,
                MPI_COMM_WORLD,
                &status
            );
            source = status.MPI_SOURCE;            
            printf("Received %d of %d from process %d\n", msg[0], msg[1], source);
        }
    } 
// Processes 1 to N-1 - senders
    else {
        msg[0] = rank;
        msg[1] = size;
        dest = 0;
        err = MPI_Send(
            msg,
            2,
            MPI_INT,
            dest,
            tag,
            MPI_COMM_WORLD
        );
    }
    err = MPI_Finalize();
    if (rank == 0) printf("Ready\n");
    exit(0);
    return 0;
}
