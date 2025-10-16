# Ejercicio 1 - Parte b: Envío de Mensajes al Proceso 0

## Descripción
Los procesos paralelos envían el mensaje "Saludos desde el proceso #" al Proceso 0, quien es responsable de imprimirlos en la salida estándar.

## Código Fuente
- `hello_world_mpi_send_recv.c`: Código fuente en C

## Compilación
```bash
mpicc hello_world_mpi_send_recv.c -o hello_world_mpi_send_recv
```

## Ejecución
```bash
# Con 4 procesos
mpirun -np 4 ./hello_world_mpi_send_recv
```

## Salida Esperada
```
Hola, soy el proceso 0 (hay 4 procesos) y recibo: (Saludos desde el proceso 1)
(Saludos desde el proceso 2)
(Saludos desde el proceso 3)
```

Nota: Los mensajes pueden aparecer en cualquier orden ya que no hay sincronización específica.