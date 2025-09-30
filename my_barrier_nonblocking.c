#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Para sleep

// Implementación de Barrier usando comunicaciones punto a punto no bloqueantes.
void My_Barrier_nonblocking(MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (size <= 1) return;

    int root = 0;
    char dummy = 'a';

    if (rank == root) {
        MPI_Request* recv_reqs = (MPI_Request*)malloc(sizeof(MPI_Request) * (size - 1));
        char* recv_dummies = (char*)malloc(sizeof(char) * (size - 1));
        int req_count = 0;
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Irecv(&recv_dummies[req_count], 1, MPI_CHAR, i, 4, comm, &recv_reqs[req_count]);
                req_count++;
            }
        }
        MPI_Waitall(req_count, recv_reqs, MPI_STATUSES_IGNORE);

        MPI_Request* send_reqs = (MPI_Request*)malloc(sizeof(MPI_Request) * (size - 1));
        req_count = 0;
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Isend(&dummy, 1, MPI_CHAR, i, 4, comm, &send_reqs[req_count++]);
            }
        }
        MPI_Waitall(req_count, send_reqs, MPI_STATUSES_IGNORE);

        free(recv_reqs);
        free(recv_dummies);
        free(send_reqs);
    } else {
        MPI_Request send_req, recv_req;
        MPI_Isend(&dummy, 1, MPI_CHAR, root, 4, comm, &send_req);
        MPI_Irecv(&dummy, 1, MPI_CHAR, root, 4, comm, &recv_req);
        MPI_Wait(&send_req, MPI_STATUS_IGNORE);
        MPI_Wait(&recv_req, MPI_STATUS_IGNORE);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Simula que cada proceso hace una cantidad de trabajo diferente
    sleep(size - rank); // Invertido para variar
    printf("Proceso %d ha terminado su trabajo y llega a la barrera.\n", rank);

    // Sincronización
    My_Barrier_nonblocking(MPI_COMM_WORLD);

    printf("Proceso %d ha pasado la barrera.\n", rank);

    MPI_Finalize();
    return 0;
}
