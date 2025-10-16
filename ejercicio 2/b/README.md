# Ejercicio 2 - Parte b: Broadcast con Comunicaciones Punto a Punto No Bloqueantes

## Descripción
Implementación de la operación colectiva Broadcast (MPI_Bcast) utilizando únicamente comunicaciones punto a punto no bloqueantes (MPI_Isend y MPI_Irecv).

## Código Fuente
- `my_bcast_nonblocking.c`: Implementación de broadcast usando comunicaciones no bloqueantes

## Compilación
```bash
mpicc my_bcast_nonblocking.c -o my_bcast_nonblocking
```

## Ejecución
```bash
# Con 4 procesos
mpirun -np 4 ./my_bcast_nonblocking
```

## Descripción del Algoritmo
- El proceso raíz inicia envíos no bloqueantes a todos los demás procesos
- Cada proceso inicia recepciones no bloqueantes
- Se utilizan MPI_Wait o MPI_Waitall para sincronizar y asegurar que todas las operaciones se completen
- Esta implementación permite mayor paralelismo y potencialmente mejor rendimiento

## Comparación con Implementación Bloqueante
Esta versión no bloqueante puede ser más eficiente ya que permite que las operaciones de comunicación se superpongan con el procesamiento, aunque requiere manejo más cuidadoso de la sincronización.