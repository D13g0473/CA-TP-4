#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Implementación de Gather usando comunicaciones punto a punto no bloqueantes.
void My_Gather_nonblocking(const void* sendbuf, int sendcount, MPI_Datatype sendtype,
                           void* recvbuf, int recvcount, MPI_Datatype recvtype,
                           int root, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int recvtype_size;
    MPI_Type_size(recvtype, &recvtype_size);

    if (rank == root) {
        MPI_Request* requests = (MPI_Request*) malloc(sizeof(MPI_Request) * (size - 1));
        int req_count = 0;
        for (int i = 0; i < size; i++) {
            if (i != root) {
                void* chunk_start = (char*)recvbuf + i * recvcount * recvtype_size;
                MPI_Irecv(chunk_start, recvcount, recvtype, i, 2, comm, &requests[req_count++]);
            }
        }
        memcpy((char*)recvbuf + root * recvcount * recvtype_size, sendbuf, sendcount * recvtype_size);
        MPI_Waitall(req_count, requests, MPI_STATUSES_IGNORE);
        free(requests);
    } else {
        MPI_Request request;
        MPI_Isend(sendbuf, sendcount, sendtype, root, 2, comm, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int root_process = 0;
    int send_value = (rank + 1) * 100;
    int* recv_array = NULL;

    if (rank == root_process) {
        recv_array = (int*)malloc(sizeof(int) * size);
    }

    printf("Proceso %d: enviando el valor %d\n", rank, send_value);

    My_Gather_nonblocking(&send_value, 1, MPI_INT, recv_array, 1, MPI_INT, root_process, MPI_COMM_WORLD);

    if (rank == root_process) {
        printf("Proceso %d: array recolectado [", rank);
        for (int i = 0; i < size; i++) {
            printf("%d%s", recv_array[i], (i == size - 1) ? "" : ", ");
        }
        printf("]\n");
        free(recv_array);
    }

    MPI_Finalize();
    return 0;
}
