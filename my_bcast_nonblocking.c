#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// Implementación de Bcast usando comunicaciones punto a punto no bloqueantes.
void My_Bcast_nonblocking(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == root) {
        MPI_Request* requests = (MPI_Request*) malloc(sizeof(MPI_Request) * (size - 1));
        int req_count = 0;
        // El proceso raíz envía los datos a todos los demás procesos.
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Isend(data, count, datatype, i, 0, comm, &requests[req_count++]);
            }
        }
        MPI_Waitall(req_count, requests, MPI_STATUSES_IGNORE);
        free(requests);
    } else {
        MPI_Request request;
        // Todos los demás procesos reciben los datos del proceso raíz.
        MPI_Irecv(data, count, datatype, root, 0, comm, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int root_process = 0;
    int number;

    if (rank == root_process) {
        number = 456; // El número a difundir
        printf("Proceso %d: difundiendo el número %d\n", rank, number);
    }

    // Llamada a nuestra implementación de Bcast no bloqueante
    My_Bcast_nonblocking(&number, 1, MPI_INT, root_process, MPI_COMM_WORLD);

    // Todos los procesos imprimen el número que recibieron
    printf("Proceso %d: el número recibido es %d\n", rank, number);

    MPI_Finalize();
    return 0;
}
