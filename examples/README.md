# Ejemplos de Configuración

Este directorio contiene ejemplos de configuración para diferentes escenarios de simulación MANET.

## Estructura de Archivos

- `simulation_params.json`: Parámetros básicos de simulación
- `scenarios/`: Diferentes escenarios de prueba
- `sample_results.csv`: Ejemplo de formato de resultados

## Uso

### 1. Parámetros de Simulación

El archivo `simulation_params.json` contiene la configuración básica:

```json
{
  "nodes": 20,
  "duration": 100,
  "speed": 10,
  "pause": 2,
  "area": {
    "width": 1000,
    "height": 1000
  }
}
```

### 2. Ejecutar Escenarios

```bash
# Escenario de baja movilidad
./ns3 run "scratch/manet-olsr --nodes=20 --speed=5 --duration=100"

# Escenario de alta movilidad
./ns3 run "scratch/manet-olsr --nodes=20 --speed=20 --duration=100"

# Escenario de alta densidad
./ns3 run "scratch/manet-olsr --nodes=50 --speed=10 --duration=150"
```

### 3. Analizar Resultados

```bash
# Analizar resultados y generar gráficos
python3 ../scripts/analyze_results.py sample_results.csv

# Solo estadísticas, sin gráficos
python3 ../scripts/analyze_results.py sample_results.csv --no-plots
```

## Escenarios de Ejemplo

### Escenario 1: Baja Movilidad
- Nodos: 20
- Velocidad: 0-5 m/s
- Pausa: 30-60 s
- Objetivo: Evaluar overhead de protocolo

### Escenario 2: Alta Movilidad
- Nodos: 20
- Velocidad: 15-20 m/s
- Pausa: 0-10 s
- Objetivo: Evaluar adaptación a cambios

### Escenario 3: Alta Densidad
- Nodos: 40-50
- Velocidad: 10 m/s
- Pausa: 10 s
- Objetivo: Evaluar escalabilidad

### Escenario 4: Tráfico Pesado
- Nodos: 25
- Velocidad: 10 m/s
- Rate: 4096 bps
- Objetivo: Evaluar throughput

## Formato de Resultados

Los resultados deben guardarse en formato CSV con las siguientes columnas:

| Columna | Descripción | Unidad |
|---------|-------------|--------|
| protocol | Nombre del protocolo | - |
| nodes | Número de nodos | - |
| speed | Velocidad de nodos | m/s |
| duration | Duración de simulación | segundos |
| pdr | Packet Delivery Ratio | % |
| delay | Delay promedio | ms |
| throughput | Throughput de red | kbps |
| overhead | Overhead de control | % |

## Mejores Prácticas

1. **Múltiples Corridas**: Ejecutar cada escenario 10-30 veces con diferentes semillas
2. **Documentación**: Documentar todos los parámetros usados
3. **Resultados**: Guardar resultados en archivos CSV con nombres descriptivos
4. **Análisis**: Usar scripts de análisis para procesar resultados
5. **Validación**: Comparar resultados con literatura publicada

## Referencias

Ver documentación completa en `/docs/SIMULACIONES.md`
