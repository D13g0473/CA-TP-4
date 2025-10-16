# Ejercicio 2 - Parte d: Implementaciones de Todas las Comunicaciones Colectivas

## Descripción
Implementaciones de todas las operaciones colectivas de MPI utilizando únicamente comunicaciones punto a punto.

## Archivos Incluidos

### Broadcast (MPI_Bcast)
- `my_bcast_blocking.c`: Versión con operaciones bloqueantes
- `my_bcast_nonblocking.c`: Versión con operaciones no bloqueantes

### All-to-All (MPI_Alltoall)
- `my_alltoall_blocking.c`: Versión bloqueante
- `my_alltoall_nonblocking.c`: Versión no bloqueante

### Gather (MPI_Gather)
- `my_gather_blocking.c`: Versión bloqueante
- `my_gather_nonblocking.c`: Versión no bloqueante

### Reduce (MPI_Reduce)
- `my_reduce_blocking.c`: Versión bloqueante
- `my_reduce_nonblocking.c`: Versión no bloqueante

### Scatter (MPI_Scatter)
- `my_scatter_blocking.c`: Versión bloqueante
- `my_scatter_nonblocking.c`: Versión no bloqueante

## Compilación General
```bash
mpicc archivo.c -o ejecutable
```

## Ejecución General
```bash
mpirun -np <num_procesos> ./ejecutable
```

## Descripción de Operaciones Colectivas

### Broadcast
- Un proceso raíz envía datos a todos los demás procesos
- Patrón: 1 → todos

### All-to-All
- Cada proceso envía datos diferentes a cada uno de los demás procesos
- Patrón: todos → todos

### Gather
- Todos los procesos envían datos a un proceso raíz
- Patrón: todos → 1

### Reduce
- Todos los procesos contribuyen con datos que se combinan (suma, máximo, etc.) en un proceso raíz
- Patrón: todos → 1 (con operación)

### Scatter
- Un proceso raíz distribuye porciones de datos a todos los demás procesos
- Patrón: 1 → todos (distribuido)

## Implementación con Punto a Punto
Todas las implementaciones utilizan únicamente:
- `MPI_Send` / `MPI_Isend` para envío
- `MPI_Recv` / `MPI_Irecv` para recepción
- `MPI_Wait` / `MPI_Waitall` para sincronización en versiones no bloqueantes

Esto demuestra cómo las operaciones colectivas de alto nivel pueden implementarse con primitivas de bajo nivel.