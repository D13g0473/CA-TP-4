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

    // For part a): assume size == N
    if (size != N) {
        if (rank == 0) printf("Para parte a), numero de procesos debe ser igual a N\n");
        MPI_Finalize();
        return 1;
    }

    // Allocate
    int *A = NULL;
    int *B = NULL;
    int *C = NULL;
    int *local_row = (int*)malloc(N * sizeof(int));
    int local_result;

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

    // Scatter rows: each process gets one row
    MPI_Scatter(A, N, MPI_INT, local_row, N, MPI_INT, 0, MPI_COMM_WORLD);

    // Bcast B
    if (rank != 0) B = (int*)malloc(N * sizeof(int));
    MPI_Bcast(B, N, MPI_INT, 0, MPI_COMM_WORLD);

    // Compute local_result = sum local_row[i] * B[i]
    local_result = 0;
    for (int i = 0; i < N; i++) {
        local_result += local_row[i] * B[i];
    }

    // Gather results
    MPI_Gather(&local_result, 1, MPI_INT, C, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Process 0 prints
    if (rank == 0) {
        printf("Resultado (Parte a):\n");
        for (int i = 0; i < N; i++) {
            printf("C[%d] = %d\n", i, C[i]);
        }
    }

    // Free
    free(local_row);
    if (rank == 0) {
        free(A);
        free(C);
    }
    free(B);

    MPI_Finalize();
    return 0;
}