#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N;
    if (rank == 0) {
        printf("Ingrese el tamaño N de la matriz NxN: ");
        scanf("%d", &N);
    }
    // Broadcast N
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Allocate on root
    int *A = NULL;
    int *B = NULL;
    int *C = NULL;
    if (rank == 0) {
        A = (int*)malloc(N * N * sizeof(int));
        B = (int*)malloc(N * sizeof(int));
        C = (int*)malloc(N * sizeof(int));

        // Generate A and B
        if (N == 6) {
            // Use example data
            int tempA[36] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36};
            int tempB[6] = {1,2,3,4,5,6};
            memcpy(A, tempA, 36 * sizeof(int));
            memcpy(B, tempB, 6 * sizeof(int));
        } else {
            // Random data
            srand(42);
            for (int i = 0; i < N * N; i++) A[i] = rand() % 10;
            for (int i = 0; i < N; i++) B[i] = rand() % 10;
        }
    }

    // Bcast B
    if (rank != 0) B = (int*)malloc(N * sizeof(int));
    MPI_Bcast(B, N, MPI_INT, 0, MPI_COMM_WORLD);

    if (N % size == 0) {
        // Case 1: rows multiple of processes
        int rows_per_proc = N / size;
        int *local_rows = (int*)malloc(rows_per_proc * N * sizeof(int));
        int *local_results = (int*)malloc(rows_per_proc * sizeof(int));

        // Scatter rows
        MPI_Scatter(A, rows_per_proc * N, MPI_INT, local_rows, rows_per_proc * N, MPI_INT, 0, MPI_COMM_WORLD);

        // Compute
        for (int r = 0; r < rows_per_proc; r++) {
            local_results[r] = 0;
            for (int i = 0; i < N; i++) {
                local_results[r] += local_rows[r * N + i] * B[i];
            }
        }

        // Gather
        MPI_Gather(local_results, rows_per_proc, MPI_INT, C, rows_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

        free(local_rows);
        free(local_results);
    } else {
        // Case 2: rows not multiple of processes
        int base = N / size;
        int remainder = N % size;
        int *sendcounts = (int*)malloc(size * sizeof(int));
        int *displs = (int*)malloc(size * sizeof(int));
        int total_displ = 0;
        for (int p = 0; p < size; p++) {
            sendcounts[p] = (p < remainder ? base + 1 : base) * N;
            displs[p] = total_displ;
            total_displ += sendcounts[p];
        }

        int local_rows_count = (rank < remainder ? base + 1 : base);
        int *local_rows = (int*)malloc(local_rows_count * N * sizeof(int));
        int *local_results = (int*)malloc(local_rows_count * sizeof(int));

        // Scatterv
        MPI_Scatterv(A, sendcounts, displs, MPI_INT, local_rows, local_rows_count * N, MPI_INT, 0, MPI_COMM_WORLD);

        // Compute
        for (int r = 0; r < local_rows_count; r++) {
            local_results[r] = 0;
            for (int i = 0; i < N; i++) {
                local_results[r] += local_rows[r * N + i] * B[i];
            }
        }

        // Gatherv
        int *recvcounts = (int*)malloc(size * sizeof(int));
        int *recvdispls = (int*)malloc(size * sizeof(int));
        total_displ = 0;
        for (int p = 0; p < size; p++) {
            recvcounts[p] = (p < remainder ? base + 1 : base);
            recvdispls[p] = total_displ;
            total_displ += recvcounts[p];
        }

        MPI_Gatherv(local_results, local_rows_count, MPI_INT, C, recvcounts, recvdispls, MPI_INT, 0, MPI_COMM_WORLD);

        free(sendcounts);
        free(displs);
        free(recvcounts);
        free(recvdispls);
        free(local_rows);
        free(local_results);
    }

    // Process 0 prints
    if (rank == 0) {
        printf("Resultado:\n");
        for (int i = 0; i < N; i++) {
            printf("C[%d] = %d\n", i, C[i]);
        }
    }

    // Free
    if (rank == 0) {
        free(A);
        free(C);
    }
    free(B);

    MPI_Finalize();
    return 0;
}