# Ejercicio 2 - Parte a: Broadcast con Comunicaciones Punto a Punto Bloqueantes

## Descripción
Implementación de la operación colectiva Broadcast (MPI_Bcast) utilizando únicamente comunicaciones punto a punto bloqueantes (MPI_Send y MPI_Recv).

## Código Fuente
- `my_bcast_blocking.c`: Implementación de broadcast usando comunicaciones bloqueantes

## Compilación
```bash
mpicc my_bcast_blocking.c -o my_bcast_blocking
```

## Ejecución
```bash
# Con 4 procesos
mpirun -np 4 ./my_bcast_blocking
```

## Descripción del Algoritmo
- El proceso raíz (0) envía el mensaje a todos los demás procesos
- Cada proceso recibe el mensaje del proceso raíz
- Se utiliza un árbol de comunicación simple donde el raíz se conecta directamente con todos los demás procesos

## Comparación con Implementación No Bloqueante
Esta implementación utiliza comunicaciones bloqueantes, lo que significa que cada operación Send/Recv espera a que se complete antes de continuar. Esto puede ser menos eficiente que las versiones no bloqueantes pero es más simple de implementar.