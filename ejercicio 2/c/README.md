# Ejercicio 2 - Parte c: Comparación entre Implementaciones Bloqueantes y No Bloqueantes

## Descripción
Análisis comparativo entre las implementaciones de comunicaciones colectivas usando operaciones bloqueantes vs no bloqueantes.

## Archivos Relacionados
- `my_bcast_blocking.c`: Broadcast con operaciones bloqueantes
- `my_bcast_nonblocking.c`: Broadcast con operaciones no bloqueantes

## Diferencias Principales

### Operaciones Bloqueantes:
- **MPI_Send/MPI_Recv**: Esperan a que la operación se complete antes de continuar
- **Ventajas**: Simplicidad de implementación, sincronización automática
- **Desventajas**: Menor paralelismo, potencial bottleneck en el proceso raíz

### Operaciones No Bloqueantes:
- **MPI_Isend/MPI_Irecv**: Inician la operación y retornan inmediatamente
- **Ventajas**: Mayor paralelismo, mejor aprovechamiento de recursos
- **Desventajas**: Necesidad de sincronización manual con MPI_Wait/MPI_Waitall

## Uso de Barreras de Sincronización
- **En implementaciones bloqueantes**: Generalmente no requieren barreras adicionales ya que la sincronización es implícita
- **En implementaciones no bloqueantes**: Pueden requerir barreras (MPI_Barrier) para asegurar que todas las operaciones no bloqueantes se completen antes de continuar con el procesamiento

## Recomendaciones
- Usar operaciones bloqueantes para algoritmos simples o cuando la sincronización es crítica
- Usar operaciones no bloqueantes para algoritmos complejos donde el paralelismo es importante
- Considerar el overhead de las operaciones no bloqueantes vs el tiempo de espera de las bloqueantes