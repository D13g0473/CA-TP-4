// Incluye la biblioteca estándar de entrada/salida para funciones como printf.
#include <stdio.h>
// Incluye la biblioteca de manipulación de cadenas para funciones como strlen.
#include <string.h>
// Incluye la biblioteca MPI para las funciones de MPI.
#include <mpi.h>

// La función principal del programa.
int main(int argc, char **argv) {
    // Variable para almacenar el rango (identificador) del proceso.
    int rank;
    // Variable para almacenar el número total de procesos.
    int size;
    // Buffer para almacenar el mensaje a enviar o recibir.
    char message[100];
    // Status de la recepción (no se usará en este ejemplo).
    MPI_Status status;

    // Inicializa el entorno MPI.
    MPI_Init(&argc, &argv);
    // Obtiene el rango del proceso actual.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Obtiene el número total de procesos.
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Si el rango del proceso no es 0 (es decir, no es el proceso maestro).
    if (rank != 0) {
        // Crea el mensaje de saludo.
        sprintf(message, "Saludos desde el proceso %d", rank);
        // Envía el mensaje al proceso 0.
        // Parámetros: buffer, tamaño del mensaje, tipo de dato, rango del destinatario, tag, comunicador.
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    } else {
        // Si el rango del proceso es 0 (es decir, es el proceso maestro).
        printf("Hola, soy el proceso 0 (hay %d procesos) y recibo: ", size);
        // Bucle para recibir mensajes de todos los demás procesos.
        for (int i = 1; i < size; i++) {
            // Recibe un mensaje del proceso 'i'.
            // Parámetros: buffer, tamaño del buffer, tipo de dato, rango del remitente, tag, comunicador, status.
            MPI_Recv(message, 100, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
            // Imprime el mensaje recibido.
            printf("(%s) \n", message);
        }
        printf("\n"); // Imprime un salto de línea al final.
    }

    // Finaliza el entorno MPI.
    MPI_Finalize();

    // Devuelve 0 para indicar una ejecución exitosa.
    return 0;
}

