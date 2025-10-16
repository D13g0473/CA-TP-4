# Ejercicio 1 - Parte a: Hello World Básico

## Descripción
Este programa implementa un saludo básico en paralelo donde cada proceso imprime "Hello World from Node X" donde X es el rango del proceso.

## Código Fuente
- `hello_world_mpi.c`: Código fuente en C

## Compilación
```bash
mpicc hello_world_mpi.c -o hello_world_mpi
```

## Ejecución
Ejecutar con diferente número de procesos para comparar salidas:
```bash
# Con 2 procesos
mpirun -np 2 ./hello_world_mpi

# Con 4 procesos
mpirun -np 4 ./hello_world_mpi

# Con 8 procesos
mpirun -np 8 ./hello_world_mpi
```

## Salida Esperada
Para 4 procesos, la salida podría ser (orden puede variar):
```
Hello World from Node 0
Hello World from Node 1
Hello World from Node 2
Hello World from Node 3
```

Analizar cómo cambia la salida con diferentes números de procesos.