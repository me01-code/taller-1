# Scripts de Simulación

Este directorio contiene scripts de simulación NS-3 y herramientas de análisis para el taller de Métodos Estocásticos.

## Contenido

### Scripts NS-3 (C++)

- **manet-olsr.cc**: Simulación MANET con protocolo OLSR
- **manet-batman.cc**: Simulación MANET con protocolo BATMAN (próximamente)
- **manet-comparison.cc**: Comparación entre protocolos (próximamente)

### Scripts de Análisis (Python)

- **analyze_results.py**: Análisis estadístico de resultados
- **consolidate_results.py**: Consolidar múltiples archivos CSV (próximamente)
- **plot_results.py**: Generación de gráficos (próximamente)

## Uso

### Scripts NS-3

#### 1. Copiar al Directorio Scratch de NS-3

```bash
# Copiar scripts a NS-3
cp scripts/*.cc ~/ns3-workspace/ns-3/scratch/
```

#### 2. Compilar y Ejecutar

```bash
cd ~/ns3-workspace/ns-3

# Simulación OLSR básica
./ns3 run scratch/manet-olsr

# Con parámetros personalizados
./ns3 run "scratch/manet-olsr \
    --nodes=30 \
    --duration=150 \
    --speed=15 \
    --pause=5"

# Con NetAnim habilitado
./ns3 run "scratch/manet-olsr --netanim=1"

# Con trazas PCAP
./ns3 run "scratch/manet-olsr --pcap=1"
```

#### 3. Parámetros Disponibles

##### manet-olsr.cc

| Parámetro | Descripción | Default | Unidad |
|-----------|-------------|---------|--------|
| `--nodes` | Número de nodos | 20 | - |
| `--duration` | Duración de simulación | 100 | segundos |
| `--speed` | Velocidad máxima | 10 | m/s |
| `--pause` | Tiempo de pausa | 2 | segundos |
| `--packetSize` | Tamaño de paquete | 512 | bytes |
| `--interval` | Intervalo entre paquetes | 0.5 | segundos |
| `--netanim` | Habilitar NetAnim | 0 | 0/1 |
| `--pcap` | Habilitar PCAP | 0 | 0/1 |

**Ejemplo completo:**
```bash
./ns3 run "scratch/manet-olsr \
    --nodes=25 \
    --duration=200 \
    --speed=12 \
    --pause=10 \
    --packetSize=1024 \
    --interval=0.25 \
    --netanim=1"
```

### Scripts de Análisis Python

#### Instalación de Dependencias

```bash
# Instalar bibliotecas necesarias
pip3 install numpy pandas matplotlib scipy
```

#### analyze_results.py

Análisis estadístico completo de resultados de simulación.

**Uso básico:**
```bash
python3 scripts/analyze_results.py results/data.csv
```

**Con opciones:**
```bash
# Especificar directorio de salida
python3 scripts/analyze_results.py results/data.csv -o results/figures/

# Sin generar gráficos
python3 scripts/analyze_results.py results/data.csv --no-plots
```

**Salida:**
- Estadísticas descriptivas
- Intervalos de confianza
- Pruebas de hipótesis
- Análisis de correlación
- Ajuste de distribuciones
- Gráficos (histogramas, scatter plots, boxplots)

## Desarrollo de Nuevos Scripts

### Plantilla para Script NS-3

```cpp
/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("MySimulation");

int main(int argc, char *argv[])
{
    // Configuración de parámetros
    uint32_t nNodes = 20;
    double simTime = 100.0;
    
    CommandLine cmd(__FILE__);
    cmd.AddValue("nodes", "Number of nodes", nNodes);
    cmd.AddValue("duration", "Simulation time (seconds)", simTime);
    cmd.Parse(argc, argv);
    
    // Crear nodos
    NodeContainer nodes;
    nodes.Create(nNodes);
    
    // Configurar WiFi, movilidad, routing, aplicaciones
    // ...
    
    // Ejecutar simulación
    Simulator::Stop(Seconds(simTime));
    Simulator::Run();
    Simulator::Destroy();
    
    return 0;
}
```

### Plantilla para Script Python

```python
#!/usr/bin/env python3
"""
Descripción del script
"""

import argparse
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def main():
    parser = argparse.ArgumentParser(description='Script description')
    parser.add_argument('input_file', help='Input CSV file')
    parser.add_argument('-o', '--output', default='output.png')
    args = parser.parse_args()
    
    # Cargar datos
    df = pd.read_csv(args.input_file)
    
    # Procesar
    # ...
    
    # Guardar resultados
    # ...

if __name__ == '__main__':
    main()
```

## Escenarios Predefinidos

### Escenario 1: Baja Movilidad

```bash
./ns3 run "scratch/manet-olsr \
    --nodes=20 \
    --speed=5 \
    --pause=30 \
    --duration=100"
```

### Escenario 2: Alta Movilidad

```bash
./ns3 run "scratch/manet-olsr \
    --nodes=20 \
    --speed=20 \
    --pause=2 \
    --duration=100"
```

### Escenario 3: Alta Densidad

```bash
./ns3 run "scratch/manet-olsr \
    --nodes=50 \
    --speed=10 \
    --pause=10 \
    --duration=150"
```

### Escenario 4: Tráfico Pesado

```bash
./ns3 run "scratch/manet-olsr \
    --nodes=25 \
    --packetSize=1024 \
    --interval=0.1 \
    --duration=100"
```

## Batch de Simulaciones

### Script Bash para Múltiples Corridas

```bash
#!/bin/bash
# run_experiments.sh

NS3_DIR=~/ns3-workspace/ns-3

# Parámetros
PROTOCOLS=("olsr" "batman")
SPEEDS=(5 10 15 20)
NODES=(20 30 40)
RUNS=10

for protocol in "${PROTOCOLS[@]}"; do
    for speed in "${SPEEDS[@]}"; do
        for nodes in "${NODES[@]}"; do
            for run in $(seq 1 $RUNS); do
                echo "Running: $protocol, $nodes nodes, ${speed}m/s, run $run"
                
                cd $NS3_DIR
                ./ns3 run "scratch/manet-$protocol \
                    --nodes=$nodes \
                    --speed=$speed \
                    --duration=100" \
                    > results/${protocol}_${nodes}n_${speed}ms_run${run}.log
            done
        done
    done
done

echo "All simulations completed!"
```

### Python para Batch con Análisis

```python
#!/usr/bin/env python3
import subprocess
import itertools
import pandas as pd

# Parámetros
nodes = [20, 30, 40]
speeds = [5, 10, 15, 20]
protocols = ['olsr', 'batman']
runs = 10

results = []

for protocol, n, speed in itertools.product(protocols, nodes, speeds):
    for run in range(runs):
        print(f"Running: {protocol}, {n} nodes, {speed} m/s, run {run+1}")
        
        cmd = [
            './ns3', 'run',
            f'scratch/manet-{protocol}',
            f'--nodes={n}',
            f'--speed={speed}',
            f'--duration=100'
        ]
        
        # Ejecutar simulación
        result = subprocess.run(cmd, capture_output=True, text=True)
        
        # Parsear resultados (ejemplo)
        # pdr = parse_pdr_from_output(result.stdout)
        # results.append({'protocol': protocol, 'nodes': n, 'speed': speed, 'pdr': pdr})

# Guardar resultados
# df = pd.DataFrame(results)
# df.to_csv('batch_results.csv', index=False)
```

## Depuración

### Habilitar Logs en NS-3

```bash
# Todos los logs
./ns3 run "scratch/manet-olsr --verbose=1"

# Logs específicos
export NS_LOG="ManetOlsrSimulation=level_all|prefix_func"
./ns3 run scratch/manet-olsr
```

### Verificar Compilación

```bash
cd ~/ns3-workspace/ns-3
./ns3 configure --enable-examples --enable-tests
./ns3 build

# Verificar errores
./test.py
```

## Mejores Prácticas

1. **Comentar código**: Documentar parámetros y lógica
2. **Validar parámetros**: Usar NS_ASSERT para validaciones
3. **Modular**: Separar funcionalidad en funciones
4. **Logging**: Usar NS_LOG para depuración
5. **Reproducibilidad**: Guardar semillas aleatorias
6. **Documentación**: Actualizar este README con cambios

## Referencias

- [NS-3 Tutorial](https://www.nsnam.org/docs/tutorial/html/)
- [NS-3 Manual](https://www.nsnam.org/docs/manual/html/)
- [OLSR Module](https://www.nsnam.org/docs/models/html/olsr.html)
- Ver documentación completa en `/docs/`

## Contribuir

Para agregar nuevos scripts:

1. Seguir convenciones de código NS-3
2. Documentar parámetros y uso
3. Agregar ejemplo de ejecución
4. Actualizar este README
5. Incluir tests si es posible

## Soporte

Para problemas con scripts:
1. Verificar que NS-3 esté correctamente instalado
2. Revisar logs de error
3. Consultar documentación
4. Contactar al profesor del curso
