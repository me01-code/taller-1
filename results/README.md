# Directorio de Resultados

Este directorio está destinado para almacenar los resultados de las simulaciones ejecutadas.

## Estructura Recomendada

```
results/
├── raw/                    # Datos crudos de simulaciones
│   ├── olsr/              # Resultados OLSR
│   └── batman/            # Resultados BATMAN
├── processed/             # Datos procesados
│   ├── statistics.csv
│   └── summary.json
├── figures/               # Gráficos generados
│   ├── pdr_comparison.png
│   ├── delay_distribution.png
│   └── throughput_vs_mobility.png
├── animations/            # Archivos NetAnim XML
│   ├── scenario1.xml
│   └── scenario2.xml
└── reports/              # Informes generados
    └── final_report.pdf
```

## Formato de Archivos

### CSV de Resultados

Estructura recomendada para archivos CSV:

```csv
protocol,nodes,speed,duration,pdr,delay,throughput,overhead,timestamp
OLSR,20,10,100,92.5,48.3,152.1,12.8,2025-11-02T10:30:00
```

**Columnas:**
- `protocol`: Nombre del protocolo (OLSR, BATMAN)
- `nodes`: Número de nodos en la simulación
- `speed`: Velocidad máxima de nodos (m/s)
- `duration`: Duración de simulación (segundos)
- `pdr`: Packet Delivery Ratio (%)
- `delay`: Delay promedio end-to-end (ms)
- `throughput`: Throughput de red (kbps)
- `overhead`: Overhead de control (%)
- `timestamp`: Marca de tiempo de la simulación

### JSON de Configuración

Guardar parámetros de simulación:

```json
{
  "simulation_id": "sim_001",
  "date": "2025-11-02",
  "protocol": "OLSR",
  "parameters": {
    "nodes": 20,
    "speed": 10,
    "area": "1000x1000",
    "duration": 100
  },
  "results": {
    "pdr": 92.5,
    "delay": 48.3,
    "throughput": 152.1
  }
}
```

## Nomenclatura de Archivos

Usar nombres descriptivos y consistentes:

```
{protocolo}_{nodos}n_{velocidad}ms_{fecha}.csv
Ejemplos:
- olsr_20n_10ms_20251102.csv
- batman_30n_15ms_20251102.csv
- comparison_20251102.csv
```

Para animaciones:
```
{protocolo}_{escenario}_{fecha}.xml
Ejemplos:
- olsr_low_mobility_20251102.xml
- batman_high_density_20251102.xml
```

## Tipos de Resultados

### 1. Resultados Crudos (Raw)

Datos directos de FlowMonitor o NS-3:
- Estadísticas por flujo
- Trazas de paquetes
- Logs de simulación

### 2. Resultados Procesados

Datos agregados y calculados:
- Métricas promedio
- Estadísticas descriptivas
- Intervalos de confianza

### 3. Visualizaciones

Gráficos generados:
- Histogramas de distribuciones
- Gráficos de comparación
- Matrices de correlación
- Series temporales

### 4. Animaciones

Archivos para NetAnim:
- Archivos XML de trazas
- Configuraciones de visualización

### 5. Informes

Documentos finales:
- Informes técnicos (PDF, LaTeX)
- Presentaciones (PowerPoint, PDF)
- Resúmenes ejecutivos

## Scripts de Procesamiento

### Consolidar Resultados

```bash
# Combinar múltiples archivos CSV
python3 ../scripts/consolidate_results.py raw/*.csv -o processed/all_results.csv
```

### Generar Estadísticas

```bash
# Análisis estadístico
python3 ../scripts/analyze_results.py processed/all_results.csv
```

### Crear Gráficos

```bash
# Generar todos los gráficos
python3 ../scripts/analyze_results.py processed/all_results.csv -o figures/
```

## Análisis de Resultados

### Verificación de Datos

Antes de analizar, verificar:

```python
import pandas as pd

# Cargar datos
df = pd.read_csv('processed/all_results.csv')

# Verificar estructura
print(df.head())
print(df.info())
print(df.describe())

# Buscar valores faltantes
print(df.isnull().sum())

# Buscar valores atípicos
print(df[df['pdr'] > 100])  # PDR no puede ser > 100%
print(df[df['pdr'] < 0])    # PDR no puede ser negativo
```

### Estadísticas Básicas

```python
# Por protocolo
print(df.groupby('protocol').mean())
print(df.groupby('protocol').std())

# Por velocidad
print(df.groupby('speed').mean())
```

### Comparaciones

```python
# Comparar protocolos
olsr_pdr = df[df['protocol'] == 'OLSR']['pdr']
batman_pdr = df[df['protocol'] == 'BATMAN']['pdr']

print(f"OLSR PDR: {olsr_pdr.mean():.2f} ± {olsr_pdr.std():.2f}")
print(f"BATMAN PDR: {batman_pdr.mean():.2f} ± {batman_pdr.std():.2f}")
```

## Backup y Versionado

### Backup Regular

```bash
# Crear backup con fecha
tar -czf backup_$(date +%Y%m%d).tar.gz results/

# Backup selectivo (solo procesados y figuras)
tar -czf backup_important_$(date +%Y%m%d).tar.gz \
    results/processed/ results/figures/
```

### Git para Resultados Pequeños

```bash
# Agregar resultados procesados (no archivos grandes)
git add results/processed/*.csv
git add results/figures/*.png
git commit -m "Add results from simulation batch 1"
```

### .gitignore para Archivos Grandes

```
# En .gitignore
results/raw/*.pcap
results/animations/*.xml
results/*.zip
results/*.tar.gz
```

## Mejores Prácticas

### 1. Documentación

Siempre incluir:
- Fecha y hora de simulación
- Parámetros usados
- Versión de NS-3
- Número de corridas
- Semillas aleatorias

### 2. Organización

- Un directorio por experimento
- Nombres descriptivos
- README en cada subdirectorio

### 3. Reproducibilidad

Guardar:
- Scripts de simulación usados
- Parámetros de configuración
- Versiones de software
- Comandos ejecutados

### 4. Validación

Antes de análisis final:
- Verificar datos completos
- Buscar anomalías
- Validar rangos de valores
- Comparar con literatura

## Plantillas

### Plantilla de Informe

```markdown
# Informe de Simulación - [Fecha]

## Configuración
- Protocolo: [OLSR/BATMAN]
- Nodos: [N]
- Velocidad: [M] m/s
- Duración: [T] segundos

## Resultados
- PDR: [X.X] %
- Delay: [Y.Y] ms
- Throughput: [Z.Z] kbps

## Análisis
[Interpretación de resultados]

## Conclusiones
[Conclusiones principales]
```

### Script de Limpieza

```bash
#!/bin/bash
# cleanup_results.sh

# Eliminar archivos temporales
find results/ -name "*.tmp" -delete
find results/ -name "*.log" -delete

# Comprimir archivos grandes
find results/animations/ -name "*.xml" -size +10M -exec gzip {} \;

echo "Limpieza completada"
```

## Recursos

- [Pandas Documentation](https://pandas.pydata.org/docs/)
- [Matplotlib Gallery](https://matplotlib.org/stable/gallery/index.html)
- [SciPy Stats](https://docs.scipy.org/doc/scipy/reference/stats.html)

## Notas

- Archivos `.xml` de NetAnim pueden ser muy grandes (>100MB)
- Considerar comprimir con `gzip` o `zip`
- Hacer backup regular de resultados importantes
- No versionar archivos muy grandes en Git
- Usar almacenamiento externo para archivos de trazas completas
