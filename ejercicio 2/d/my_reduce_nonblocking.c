#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// Implementación de Reduce (SUM) usando comunicaciones punto a punto no bloqueantes.
void My_Reduce_nonblocking(const void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
                           MPI_Op op, int root, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (op != MPI_SUM || datatype != MPI_INT || count != 1) {
        if (rank == 0) printf("Error: Esta implementación simple solo soporta MPI_SUM en un único MPI_INT.\n");
        MPI_Abort(comm, 1);
    }

    int send_value = *((int*)sendbuf);

    if (rank == root) {
        int* temp_values = (int*)malloc(sizeof(int) * (size - 1));
        MPI_Request* requests = (MPI_Request*)malloc(sizeof(MPI_Request) * (size - 1));
        int req_count = 0;

        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Irecv(&temp_values[req_count], 1, MPI_INT, i, 3, comm, &requests[req_count]);
                req_count++;
            }
        }

        MPI_Waitall(req_count, requests, MPI_STATUSES_IGNORE);

        int total_sum = send_value;
        for (int i = 0; i < req_count; i++) {
            total_sum += temp_values[i];
        }
        *((int*)recvbuf) = total_sum;

        free(temp_values);
        free(requests);
    } else {
        MPI_Request request;
        MPI_Isend(&send_value, 1, MPI_INT, root, 3, comm, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int root_process = 0;
    int my_value = rank + 1;
    int total_sum;

    printf("Proceso %d: mi valor es %d\n", rank, my_value);

    My_Reduce_nonblocking(&my_value, &total_sum, 1, MPI_INT, MPI_SUM, root_process, MPI_COMM_WORLD);

    if (rank == root_process) {
        printf("Proceso %d: la suma total reducida es %d\n", rank, total_sum);
    }

    MPI_Finalize();
    return 0;
}
