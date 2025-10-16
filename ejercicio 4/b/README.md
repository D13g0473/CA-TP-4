# Ejercicio 4 - Parte b: Multiplicación Matriz-Vector (Caso General)

## Descripción
Implementación paralela de la multiplicación de una matriz por un vector que maneja casos donde el número de filas puede ser diferente al número de procesos.

## Código Fuente
- `multiplicacion_matriz_vector_b.c`: Implementación de la Parte b)

## Casos Manejados

### Caso 1: N divisible por número de procesos
- Las filas se distribuyen equitativamente entre los procesos
- Cada proceso maneja N/P filas (donde P es el número de procesos)

### Caso 2: N no divisible por número de procesos
- Algunos procesos manejan una fila más que otros
- Distribución lo más equitativa posible

## Algoritmo General
1. **Proceso 0** genera la matriz y el vector
2. Se calcula cuántas filas maneja cada proceso
3. Las filas correspondientes se distribuyen a cada proceso
4. El vector se replica en todos los procesos
5. Cada proceso realiza las multiplicaciones de sus filas asignadas
6. Los resultados parciales se reúnen en el proceso 0

## Compilación
```bash
mpicc multiplicacion_matriz_vector_b.c -o multiplicacion_b
```

## Ejecución
```bash
# Sintaxis: echo "N" | mpirun -np P ./multiplicacion_b
# donde P < N (número de procesos menor que filas)

# Caso 1: N=8, divisible por 4 procesos
echo "8" | mpirun -np 4 ./multiplicacion_b

# Caso 2: N=7, no divisible por 4 procesos
echo "7" | mpirun -np 4 ./multiplicacion_b
```

## Distribución de Trabajo

### Ejemplo: N=8, P=4 procesos
- Proceso 0: filas 0, 1
- Proceso 1: filas 2, 3
- Proceso 2: filas 4, 5
- Proceso 3: filas 6, 7

### Ejemplo: N=7, P=4 procesos
- Proceso 0: filas 0, 1 (2 filas)
- Proceso 1: fila 2 (1 fila)
- Proceso 2: filas 3, 4 (2 filas)
- Proceso 3: filas 5, 6 (2 filas)

## Ventajas sobre Parte a)
- Funciona con cualquier número de procesos menor o igual al número de filas
- Mejor aprovechamiento de recursos cuando P < N
- Manejo automático de distribuciones desiguales

## Limitación
- Requiere que P ≤ N (número de procesos ≤ número de filas)