#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// Implementación de Bcast usando comunicaciones punto a punto bloqueantes.
void My_Bcast_blocking(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == root) {
        // El proceso raíz envía los datos a todos los demás procesos.
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Send(data, count, datatype, i, 0, comm);
            }
        }
    } else {
        // Todos los demás procesos reciben los datos del proceso raíz.
        MPI_Recv(data, count, datatype, root, 0, comm, MPI_STATUS_IGNORE);
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
        number = 123; // El número a difundir
        printf("Proceso %d: difundiendo el número %d\n", rank, number);
    }

    // Llamada a nuestra implementación de Bcast
    My_Bcast_blocking(&number, 1, MPI_INT, root_process, MPI_COMM_WORLD);

    // Todos los procesos imprimen el número que recibieron
    printf("Proceso %d: el número recibido es %d\n", rank, number);

    MPI_Finalize();
    return 0;
}
