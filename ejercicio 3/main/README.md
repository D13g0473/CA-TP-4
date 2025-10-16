# Ejercicio 3: Programa Anillo

## Descripción
Programa MPI que implementa la circulación de un mensaje a través de un "anillo lógico" de procesos. Los procesos pasan un mensaje en secuencia circular tantas veces como se especifique.

## Código Fuente
- `anillo.c`: Implementación del programa anillo

## Algoritmo
Para 4 procesos y N vueltas:
1. Proceso 0 envía mensaje "A" al proceso 1
2. Proceso 1 recibe de 0, envía a 2
3. Proceso 2 recibe de 1, envía a 3
4. Proceso 3 recibe de 2, envía a 0
5. Se repite el ciclo N veces

## Compilación
```bash
mpicc anillo.c -o anillo
```

## Ejecución
```bash
# Sintaxis: mpirun -np <num_procesos> ./anillo <num_vueltas>
mpirun -np 4 ./anillo 2

# Ejemplo con 5 procesos y 3 vueltas
mpirun -np 5 ./anillo 3
```

## Parámetros
- `num_procesos`: Número de procesos en el anillo (mínimo 2)
- `num_vueltas`: Número de veces que el mensaje completa el circuito

## Salida Esperada
El programa muestra el paso del mensaje entre procesos en cada vuelta del anillo.

## Consideraciones
- Los procesos deben formar un anillo lógico: P0 → P1 → P2 → ... → P(n-1) → P0
- El mensaje se pasa de manera síncrona entre procesos adyacentes
- Cada proceso conoce únicamente a sus vecinos inmediatos en el anillo