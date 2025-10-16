# Ejercicio 4 - Parte a: Multiplicación Matriz-Vector (Caso Simple)

## Descripción
Implementación paralela de la multiplicación de una matriz por un vector donde cada proceso maneja exactamente una fila de la matriz.

## Código Fuente
- `multiplicacion_matriz_vector_a.c`: Implementación de la Parte a)

## Algoritmo
1. **Proceso 0** genera la matriz y el vector
2. Las filas de la matriz se distribuyen entre los procesos participantes
3. El vector se replica en todos los procesos
4. Cada proceso realiza la multiplicación de su fila por el vector completo
5. El proceso 0 reúne todos los resultados parciales

## Requisitos
- Número de procesos debe ser igual al tamaño N de la matriz
- Cada proceso maneja exactamente una fila

## Compilación
```bash
mpicc multiplicacion_matriz_vector_a.c -o multiplicacion_a
```

## Ejecución
```bash
# Sintaxis: echo "N" | mpirun -np N ./multiplicacion_a
# Ejemplo con N=6 (requiere exactamente 6 procesos)
echo "6" | mpirun -np 6 ./multiplicacion_a
```

## Resultado Esperado (N=6)
```
C[0] = 91
C[1] = 217
C[2] = 343
C[3] = 469
C[4] = 595
C[5] = 721
```

## Datos de Ejemplo
- **Matriz A (6x6)**:
  ```
  1  2  3  4  5  6
  7  8  9 10 11 12
  13 14 15 16 17 18
  19 20 21 22 23 24
  25 26 27 28 29 30
  31 32 33 34 35 36
  ```
- **Vector B (6)**: [1, 2, 3, 4, 5, 6]

## Limitaciones
- Solo funciona cuando el número de procesos es igual al número de filas
- Para casos más generales, ver Parte b)