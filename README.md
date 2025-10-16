# Ejercicio 5 - Parte b: Implementaciones del Juego de la Vida

## Descripción
Implementaciones del Juego de la Vida de Conway, tanto en versión secuencial como paralela utilizando MPI.

## Estado Actual
**Nota: Las implementaciones del Juego de la Vida no están incluidas en los archivos actuales del proyecto.**

## Requisitos de Implementación

### Versión Secuencial
- Implementación en pseudocódigo
- Estado inicial generado aleatoriamente
- Parámetros configurables: tamaño de matriz N×N y número de ciclos
- Aplicación correcta de las reglas de Conway

### Versión Paralela
- Implementación en pseudocódigo utilizando MPI
- Distribución de la matriz entre múltiples procesos
- Manejo de comunicaciones para bordes entre subregiones
- Sincronización entre generaciones

## Reglas del Juego de la Vida

### Estados de las Células
- **0**: Célula muerta
- **1**: Célula viva

### Reglas de Transición
1. **Nacimiento**: Una célula muerta con exactamente 3 vecinos vivos → nace (se vuelve viva)
2. **Supervivencia**: Una célula viva con 2 ó 3 vecinos vivos → sobrevive
3. **Muerte**: Una célula viva con menos de 2 vecinos (soledad) o más de 3 (sobrepoblación) → muere

### Vecindad
- Cada célula tiene 8 vecinos (incluyendo diagonales)
- La matriz es toroidal (bordes conectados) o con condiciones de contorno fijas

## Desafíos de Implementación Paralela

### 1. **Distribución de Datos**
- Dividir la matriz N×N entre P procesos
- Considerar divisiones por filas, columnas o bloques 2D

### 2. **Comunicaciones**
- Intercambio de franjas de datos en los bordes
- Comunicación con procesos vecinos (hasta 4 direcciones en división 2D)

### 3. **Sincronización**
- Barreras entre generaciones para asegurar actualización simultánea
- Evitar condiciones de carrera

### 4. **Balance de Carga**
- Distribución equitativa del trabajo
- Manejo de casos donde N no es divisible por P

## Pseudocódigo General (Versión Paralela)

```
INICIALIZACIÓN MPI
Obtener rank y tamaño del comunicador

PROCESO 0:
    Leer parámetros N, num_ciclos
    Generar matriz inicial aleatoria
    Distribuir porciones de matriz a otros procesos

TODOS LOS PROCESOS:
    Recibir porción asignada de la matriz
    Para cada ciclo:
        Intercambiar bordes con procesos vecinos
        Calcular nueva generación para células locales
        Sincronizar con barrera
        Intercambiar resultados si necesario

PROCESO 0:
    Recopilar resultados finales
    Mostrar estado final de la matriz

FINALIZAR MPI
```

## Archivos Esperados
- `juego_vida_secuencial.pseudo`: Pseudocódigo versión secuencial
- `juego_vida_paralelo.pseudo`: Pseudocódigo versión paralela
- `juego_vida_mpi.c`: Implementación real en C con MPI (futuro)

## Consideraciones para MPI
- Usar `MPI_Scatter` y `MPI_Gather` para distribución/recolecta
- Usar `MPI_Sendrecv` para intercambios de bordes
- Usar `MPI_Barrier` para sincronización entre generaciones
- Optimizar comunicaciones minimizando overhead