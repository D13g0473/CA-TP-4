#include <mpi.h>
#include <stdio.h>
#include <unistd.h> // Para sleep

// Implementación de Barrier usando comunicaciones punto a punto bloqueantes.
void My_Barrier_blocking(MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (size <= 1) return;

    int root = 0;
    char dummy = 'a'; // Dato ficticio para enviar/recibir

    if (rank == root) {
        // El raíz recibe de todos los demás.
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Recv(&dummy, 1, MPI_CHAR, i, 4, comm, MPI_STATUS_IGNORE);
            }
        }
        // Después de recibir de todos, el raíz envía a todos para liberarlos.
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Send(&dummy, 1, MPI_CHAR, i, 4, comm);
            }
        }
    } else {
        // Los otros procesos envían al raíz y luego esperan la respuesta.
        MPI_Send(&dummy, 1, MPI_CHAR, root, 4, comm);
        MPI_Recv(&dummy, 1, MPI_CHAR, root, 4, comm, MPI_STATUS_IGNORE);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Simula que cada proceso hace una cantidad de trabajo diferente
    sleep(rank);
    printf("Proceso %d ha terminado su trabajo y llega a la barrera.\n", rank);

    // Sincronización
    My_Barrier_blocking(MPI_COMM_WORLD);

    printf("Proceso %d ha pasado la barrera.\n", rank);

    MPI_Finalize();
    return 0;
}
