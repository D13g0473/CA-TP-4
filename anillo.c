#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, size;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Parse command line argument for n
    if (argc < 2) {
        if (rank == 0) {
            fprintf(stderr, "Usage: mpirun -np <num_processes> %s <n>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        if (rank == 0) {
            fprintf(stderr, "Error: n must be a positive integer.\n");
        }
        MPI_Finalize();
        return 1;
    }

    // Define next and previous ranks for the ring
    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    // Message to pass around the ring
    char msg = 'A';

    // Loop for n turns
    for (int turn = 0; turn < n; turn++) {
        if (rank == 0) {
            // Process 0 sends first, then receives
            printf("El proceso %d envía '%c' al proceso %d en la vuelta %d\n", rank, msg, next, turn);
            fflush(stdout);
            MPI_Send(&msg, 1, MPI_CHAR, next, 0, MPI_COMM_WORLD);
            MPI_Recv(&msg, 1, MPI_CHAR, prev, 0, MPI_COMM_WORLD, &status);
            printf("El proceso %d recibió '%c' en la vuelta %d\n", rank, msg, turn);
            fflush(stdout);
        } else {
            // Other processes receive first, then send
            MPI_Recv(&msg, 1, MPI_CHAR, prev, 0, MPI_COMM_WORLD, &status);
            printf("El proceso %d recibió '%c' en la vuelta %d\n", rank, msg, turn);
            fflush(stdout);
            printf("El proceso %d envía '%c' al proceso %d en la vuelta %d\n", rank, msg, next, turn);
            fflush(stdout);
            MPI_Send(&msg, 1, MPI_CHAR, next, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}