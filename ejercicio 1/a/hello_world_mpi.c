// Incluye la biblioteca estándar de entrada/salida para funciones como printf.
#include <stdio.h>
// Incluye la biblioteca MPI para las funciones de MPI.
#include <mpi.h>

// La función principal del programa.
int main(int argc, char **argv) {
    // Variable para almacenar el rango del proceso.
    int node;

    // Inicializa el entorno MPI. Debe ser llamado antes que cualquier otra función MPI.
    // Toma los argumentos de la línea de comandos y permite a MPI procesar los argumentos que necesite.
    MPI_Init(&argc, &argv);

    // Obtiene el rango del proceso actual dentro del comunicador MPI_COMM_WORLD.
    // El rango es un identificador único para cada proceso, desde 0 hasta (número de procesos - 1).
    MPI_Comm_rank(MPI_COMM_WORLD, &node);

    // Imprime un mensaje "Hello World" incluyendo el rango del proceso.
    printf("Hello World from Node %d\n", node);

    // Finaliza el entorno MPI. Debe ser llamado por cada proceso antes de salir.
    MPI_Finalize();

    // Devuelve 0 para indicar una ejecución exitosa.
    return 0;
}

