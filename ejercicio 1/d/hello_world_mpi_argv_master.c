// Incluye las bibliotecas necesarias.
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Necesaria para atoi()
#include <mpi.h>

// La función principal del programa.
int main(int argc, char **argv) {
    // Variables para rango, tamaño, mensaje y status.
    int rank;
    int size;
    int master_rank;
    char message[100];
    MPI_Status status;

    // Inicializa el entorno MPI.
    MPI_Init(&argc, &argv);
    // Obtiene el rango y el tamaño.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // --- MODIFICACIÓN PARA LEER DE CONSOLA ---

    // 1. Verificar si se pasó el argumento. Solo el proceso 0 imprime el error.
    if (argc < 2) {
        if (rank == 0) {
            fprintf(stderr, "Error: Debes especificar el rango del proceso maestro.\n");
            fprintf(stderr, "Uso: mpirun -np <num_procesos> %s <rango_maestro>\n", argv[0]);
        }
        // Aborta la ejecución de MPI en todos los procesos.
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // 2. Convertir el argumento (texto) a un entero.
    master_rank = atoi(argv[1]);

    // 3. Validar que el rango del maestro esté dentro de los límites.
    if (master_rank < 0 || master_rank >= size) {
        if (rank == 0) {
            fprintf(stderr, "Error: Rango de maestro inválido: %d. Debe estar entre 0 y %d.\n", master_rank, size - 1);
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // El resto de la lógica es idéntica a la del programa anterior.
    if (rank != master_rank) {
        sprintf(message, "Saludos desde el proceso %d", rank);
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, master_rank, 0, MPI_COMM_WORLD);
    } else {
        printf("Hola, soy el proceso %d (maestro designado por consola) y recibo:\n", rank);
        for (int i = 0; i < size; i++) {
            if (i != master_rank) {
                MPI_Recv(message, 100, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
                printf(" -> Recibido: (%s)\n", message);
            }
        }
    }

    // Finaliza el entorno MPI.
    MPI_Finalize();

    // Devuelve 0 para indicar una ejecución exitosa.
    return 0;
}
