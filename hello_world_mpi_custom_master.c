// Incluye las bibliotecas necesarias.
#include <stdio.h>
#include <string.h>
#include <mpi.h>

// La función principal del programa.
int main(int argc, char **argv) {
    // Variables para rango, tamaño, mensaje y status.
    int rank;
    int size;
    char message[100];
    MPI_Status status;

    // Inicializa el entorno MPI.
    MPI_Init(&argc, &argv);
    // Obtiene el rango y el tamaño.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // --- MODIFICACIÓN PRINCIPAL ---
    // Se define el rango del proceso que actuará como maestro.
    // Para este ejemplo, elegimos el último proceso disponible.
    int master_rank = size - 1;

    // Si el proceso actual NO es el maestro, entonces es un trabajador.
    if (rank != master_rank) {
        // Crea el mensaje de saludo.
        sprintf(message, "Saludos desde el proceso %d", rank);
        // Envía el mensaje al proceso maestro que hemos definido.
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, master_rank, 0, MPI_COMM_WORLD);
    } else {
        // Este bloque de código solo lo ejecutará el proceso maestro.
        printf("Hola, soy el proceso %d (el nuevo maestro, con %d procesos en total) y recibo:\n", rank, size);
        
        // Bucle para recibir mensajes de todos los demás procesos (los trabajadores).
        // Iteramos sobre todos los rangos posibles.
        for (int i = 0; i < size; i++) {
            // El maestro no necesita recibir un mensaje de sí mismo.
            if (i != master_rank) {
                // Recibe un mensaje del proceso 'i'.
                MPI_Recv(message, 100, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
                // Imprime el mensaje recibido para confirmar.
                printf(" -> Recibido: (%s)\n", message);
            }
        }
    }

    // Finaliza el entorno MPI.
    MPI_Finalize();

    // Devuelve 0 para indicar una ejecución exitosa.
    return 0;
}
