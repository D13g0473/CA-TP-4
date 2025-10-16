# Ejercicio 1 - Parte c: Mensajes en Orden Ascendente

## Descripción
Modificación del programa anterior para que los mensajes enviados al Proceso 0 se impriman en orden ascendente.

## Código Fuente
- `send_to_process_0_first.c`: Código fuente en C

## Compilación
```bash
mpicc send_to_process_0_first.c -o send_to_process_0_first
```

## Ejecución
```bash
# Con 4 procesos
mpirun -np 4 ./send_to_process_0_first
```

## Salida Esperada
```
Hola, soy el proceso 0 (hay 4 procesos) y recibo: (Saludos desde el proceso 1)
(Saludos desde el proceso 2)
(Saludos desde el proceso 3)
(Saludos desde el proceso 4)
```

Nota: Los mensajes se reciben en orden ascendente debido a la implementación del bucle de recepción.