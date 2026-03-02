/* Provoke pending communication between MPI processes 

   Author : Venkata Ayyalasomayajula 
   Email  : venkata.ayyalasomayajula@gmail.com */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0)
            printf("Run with at least 2 processes\n");
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        int send_value = 123;
        sleep(2);
        printf("Rank 0: Sending %d \n", send_value);
        MPI_Send(&send_value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
     }

    MPI_Request request;
    MPI_Status status;

    if (rank == 1) {

        int recv_value;

        printf("Rank 1: Posting non-blocking receive ...\n");
        MPI_Irecv(&recv_value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);

        sleep(4);  // simulate computation while receive is pending

        if (request != MPI_REQUEST_NULL) {
                printf("Rank 1: Cancelling receive ...\n");
                MPI_Cancel(&request);
        }

        printf("Rank 1: Freeing resource immediately after cancel ...\n");
        MPI_Request_free(&request);

    }

    MPI_Finalize();
    return 0;
}
