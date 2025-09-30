#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Implementación de Scatter usando comunicaciones punto a punto no bloqueantes.
void My_Scatter_nonblocking(const void* sendbuf, int sendcount, MPI_Datatype sendtype,
                            void* recvbuf, int recvcount, MPI_Datatype recvtype,
                            int root, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int sendtype_size;
    MPI_Type_size(sendtype, &sendtype_size);

    if (rank == root) {
        MPI_Request* requests = (MPI_Request*) malloc(sizeof(MPI_Request) * (size - 1));
        int req_count = 0;
        for (int i = 0; i < size; i++) {
            if (i != root) {
                const void* chunk_start = (const char*)sendbuf + i * sendcount * sendtype_size;
                MPI_Isend(chunk_start, sendcount, sendtype, i, 1, comm, &requests[req_count++]);
            }
        }
        memcpy(recvbuf, (const char*)sendbuf + root * sendcount * sendtype_size, recvcount * sendtype_size);
        MPI_Waitall(req_count, requests, MPI_STATUSES_IGNORE);
        free(requests);
    } else {
        MPI_Request request;
        MPI_Irecv(recvbuf, recvcount, recvtype, root, 1, comm, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int root_process = 0;
    int* send_array = NULL;
    int recv_value;

    if (rank == root_process) {
        send_array = (int*)malloc(sizeof(int) * size);
        printf("Proceso %d: distribuyendo el array [", rank);
        for (int i = 0; i < size; i++) {
            send_array[i] = (i + 1) * 100;
            printf("%d%s", send_array[i], (i == size - 1) ? "" : ", ");
        }
        printf("]\n");
    }

    My_Scatter_nonblocking(send_array, 1, MPI_INT, &recv_value, 1, MPI_INT, root_process, MPI_COMM_WORLD);

    printf("Proceso %d: recibí el valor %d\n", rank, recv_value);

    if (rank == root_process) {
        free(send_array);
    }

    MPI_Finalize();
    return 0;
}
