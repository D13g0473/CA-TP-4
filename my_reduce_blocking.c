#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// Implementación de Reduce (SUM) usando comunicaciones punto a punto bloqueantes.
void My_Reduce_blocking(const void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
                        MPI_Op op, int root, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    // Esta implementación simple solo funciona para MPI_SUM con un solo entero.
    if (op != MPI_SUM || datatype != MPI_INT || count != 1) {
        if (rank == 0) printf("Error: Esta implementación simple solo soporta MPI_SUM en un único MPI_INT.\n");
        MPI_Abort(comm, 1);
    }

    int send_value = *((int*)sendbuf);

    if (rank == root) {
        int total_sum = send_value;
        int received_value;
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Recv(&received_value, 1, MPI_INT, i, 3, comm, MPI_STATUS_IGNORE);
                total_sum += received_value;
            }
        }
        *((int*)recvbuf) = total_sum;
    } else {
        MPI_Send(&send_value, 1, MPI_INT, root, 3, comm);
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

    My_Reduce_blocking(&my_value, &total_sum, 1, MPI_INT, MPI_SUM, root_process, MPI_COMM_WORLD);

    if (rank == root_process) {
        printf("Proceso %d: la suma total reducida es %d\n", rank, total_sum);
    }

    MPI_Finalize();
    return 0;
}
