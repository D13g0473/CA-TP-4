# Ejercicio 1 - Parte d: Proceso Maestro Configurable

## Descripción
Modificación del programa para que el proceso responsable de imprimir sea otro proceso distinto del Proceso 0, especificado por línea de comandos.

## Código Fuente
- `hello_world_mpi_argv_master.c`: Código fuente en C

## Compilación
```bash
mpicc hello_world_mpi_argv_master.c -o argv_master_executable
```

## Ejecución
Especificar el rango del proceso maestro como argumento:
```bash
# Proceso 2 como maestro con 4 procesos totales
mpirun -np 4 ./argv_master_executable 2

# Proceso 1 como maestro con 5 procesos totales
mpirun -np 5 ./argv_master_executable 1
```

## Salida Esperada (ejemplo con maestro=2, 4 procesos)
```
Hola, soy el proceso 2 (maestro designado por consola) y recibo:
 -> Recibido: (Saludos desde el proceso 0)
 -> Recibido: (Saludos desde el proceso 1)
 -> Recibido: (Saludos desde el proceso 3)
```

## Validaciones
- El programa valida que se proporcione el argumento del maestro
- Verifica que el rango del maestro esté dentro de los límites válidos (0 a num_procesos-1)
- Aborta la ejecución si hay errores de validación