# Guía de Simulaciones - Protocolos MANET

Esta guía explica cómo ejecutar y analizar simulaciones de protocolos MANET (OLSR y BATMAN) en NS-3.

## Índice

1. [Introducción a MANET](#introducción-a-manet)
2. [Protocolo OLSR](#protocolo-olsr)
3. [Protocolo BATMAN](#protocolo-batman)
4. [Configuración de Simulaciones](#configuración-de-simulaciones)
5. [Ejecución de Simulaciones](#ejecución-de-simulaciones)
6. [Análisis de Resultados](#análisis-de-resultados)

## Introducción a MANET

Las redes móviles ad-hoc (MANET) son redes descentralizadas donde los nodos se comunican sin infraestructura fija. Características principales:

- **Topología Dinámica**: Los nodos se mueven libremente
- **Auto-organización**: No requiere infraestructura centralizada
- **Multi-hop**: Los paquetes pueden atravesar múltiples nodos
- **Recursos Limitados**: Batería, ancho de banda, procesamiento

### Desafíos en MANET

1. Enrutamiento dinámico
2. Gestión de movilidad
3. Eficiencia energética
4. Escalabilidad
5. Seguridad

## Protocolo OLSR

### Descripción

OLSR (Optimized Link State Routing) es un protocolo proactivo de enrutamiento para redes móviles ad-hoc.

### Características Principales

- **Tipo**: Proactivo (tabla-driven)
- **Algoritmo**: Link State con MPR (Multi-Point Relay)
- **Overhead**: Moderado a alto
- **Convergencia**: Rápida
- **Escalabilidad**: Media

### Mensajes de Control

1. **HELLO**: Detecta vecinos y calcula MPRs
2. **TC (Topology Control)**: Difunde información de topología
3. **MID (Multiple Interface Declaration)**: Para nodos con múltiples interfaces
4. **HNA (Host and Network Association)**: Anuncia redes externas

### Parámetros Configurables

```cpp
// Intervalo de mensajes HELLO
Config::SetDefault("ns3::olsr::RoutingProtocol::HelloInterval", 
                   TimeValue(Seconds(2.0)));

// Intervalo de mensajes TC
Config::SetDefault("ns3::olsr::RoutingProtocol::TcInterval", 
                   TimeValue(Seconds(5.0)));

// Tiempo de validez de la topología
Config::SetDefault("ns3::olsr::RoutingProtocol::TopologyHoldTime", 
                   TimeValue(Seconds(15.0)));
```

### Ventajas

- Rutas siempre disponibles
- Baja latencia para el primer paquete
- Bueno para tráfico constante

### Desventajas

- Overhead constante de control
- Uso continuo de ancho de banda
- Mayor consumo energético

## Protocolo BATMAN

### Descripción

BATMAN (Better Approach To Mobile Adhoc Networking) es un protocolo de enrutamiento descentralizado y proactivo.

### Características Principales

- **Tipo**: Proactivo con características híbridas
- **Algoritmo**: Basado en calidad de enlace
- **Overhead**: Bajo a moderado
- **Convergencia**: Rápida
- **Escalabilidad**: Alta

### Principio de Funcionamiento

BATMAN utiliza un enfoque descentralizado donde:
1. Cada nodo envía OGMs (Originator Messages) periódicamente
2. Los nodos reenvían OGMs de otros
3. La mejor ruta se determina por la calidad del enlace
4. No se construye una topología completa en cada nodo

### Ventajas

- Menor overhead que OLSR
- Buena adaptación a cambios de topología
- Escalable para redes grandes
- Implementación más simple

### Desventajas

- Puede tener rutas subóptimas
- Depende de la calidad del enlace
- Requiere ajuste de parámetros

## Configuración de Simulaciones

### Estructura Básica de una Simulación

```cpp
// 1. Crear nodos
NodeContainer nodes;
nodes.Create(numberOfNodes);

// 2. Configurar canal WiFi
WifiHelper wifi;
WifiMacHelper wifiMac;
YansWifiPhyHelper wifiPhy;
YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();

// 3. Configurar protocolo de enrutamiento (OLSR o BATMAN)
OlsrHelper olsr;
// o
BatmanHelper batman;

// 4. Configurar movilidad
MobilityHelper mobility;
mobility.SetMobilityModel("ns3::RandomWaypointMobilityModel");

// 5. Configurar aplicaciones
UdpEchoServerHelper echoServer(9);
UdpEchoClientHelper echoClient(serverInterface.GetAddress(0), 9);

// 6. Configurar trazas
wifiPhy.EnablePcap("manet", devices);
AnimationInterface anim("manet-animation.xml");
```

### Parámetros de Simulación

#### Área de Simulación
- Tamaño: 1000m x 1000m (ajustable)
- Rango de transmisión: 250m típico

#### Nodos
- Cantidad: 10-50 nodos
- Velocidad: 0-20 m/s
- Tiempo de pausa: 0-60 segundos

#### Tráfico
- Tipo: CBR (Constant Bit Rate)
- Tamaño de paquete: 512-1024 bytes
- Intervalo: 0.1-1.0 segundos

## Ejecución de Simulaciones

### Simular OLSR

```bash
cd ~/ns3-workspace/ns-3

# Copiar script
cp ~/taller-1/scripts/manet-olsr.cc scratch/

# Compilar y ejecutar
./ns3 run "scratch/manet-olsr --nodes=20 --duration=100"

# Con parámetros personalizados
./ns3 run "scratch/manet-olsr \
    --nodes=30 \
    --duration=200 \
    --speed=10 \
    --pause=10 \
    --rate=2048"
```

### Simular BATMAN

```bash
cd ~/ns3-workspace/ns-3

# Copiar script
cp ~/taller-1/scripts/manet-batman.cc scratch/

# Compilar y ejecutar
./ns3 run "scratch/manet-batman --nodes=20 --duration=100"

# Con NetAnim
./ns3 run "scratch/manet-batman --netanim=1"
```

### Simulación Comparativa

```bash
# Ejecutar ambos protocolos con mismos parámetros
./ns3 run "scratch/manet-comparison \
    --protocol1=OLSR \
    --protocol2=BATMAN \
    --nodes=25 \
    --duration=150"
```

## Análisis de Resultados

### Métricas Principales

#### 1. Packet Delivery Ratio (PDR)

```
PDR = (Paquetes Recibidos / Paquetes Enviados) × 100%
```

**Interpretación:**
- PDR > 90%: Excelente
- PDR 70-90%: Bueno
- PDR < 70%: Necesita optimización

#### 2. End-to-End Delay

```
Delay promedio = Σ(Tiempo_recepción - Tiempo_envío) / N
```

**Valores típicos:**
- < 100 ms: Excelente
- 100-300 ms: Aceptable
- > 300 ms: Alto

#### 3. Throughput

```
Throughput = (Bits recibidos / Tiempo de simulación)
```

#### 4. Routing Overhead

```
Overhead = (Paquetes de control / Total de paquetes) × 100%
```

### Análisis con FlowMonitor

```cpp
// Habilitar FlowMonitor en la simulación
FlowMonitorHelper flowmon;
Ptr<FlowMonitor> monitor = flowmon.InstallAll();

// Al final de la simulación
monitor->CheckForLostPackets();
Ptr<Ipv4FlowClassifier> classifier = 
    DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());

// Obtener estadísticas
std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();
```

### Visualización con NetAnim

```bash
# Abrir NetAnim
cd ~/ns3-workspace/ns-3/netanim
./NetAnim

# Cargar archivo de animación
# File -> Open -> Seleccionar manet-animation.xml

# Controles útiles:
# - Play/Pause: Reproducir animación
# - Speed: Ajustar velocidad
# - Node Trajectory: Mostrar trayectorias
# - Packet Stats: Estadísticas de paquetes
```

### Scripts de Análisis Python

```python
import pandas as pd
import matplotlib.pyplot as plt

# Cargar resultados
df = pd.read_csv('results/simulation_results.csv')

# Graficar PDR vs Movilidad
plt.figure(figsize=(10, 6))
plt.plot(df['speed'], df['pdr_olsr'], label='OLSR', marker='o')
plt.plot(df['speed'], df['pdr_batman'], label='BATMAN', marker='s')
plt.xlabel('Velocidad de Nodos (m/s)')
plt.ylabel('Packet Delivery Ratio (%)')
plt.title('PDR vs Movilidad')
plt.legend()
plt.grid(True)
plt.savefig('pdr_comparison.png')
```

### Análisis Estadístico

```python
import numpy as np
from scipy import stats

# Comparar dos protocolos
olsr_delays = np.array([...])  # Delays de OLSR
batman_delays = np.array([...])  # Delays de BATMAN

# Test t de Student
t_stat, p_value = stats.ttest_ind(olsr_delays, batman_delays)

print(f"Estadística t: {t_stat}")
print(f"Valor p: {p_value}")

if p_value < 0.05:
    print("Diferencia estadísticamente significativa")
```

## Escenarios de Prueba

### Escenario 1: Baja Movilidad
- Velocidad: 0-5 m/s
- Pausa: 30-60 segundos
- Objetivo: Evaluar overhead de control

### Escenario 2: Alta Movilidad
- Velocidad: 15-20 m/s
- Pausa: 0-10 segundos
- Objetivo: Evaluar adaptación a cambios

### Escenario 3: Alta Densidad
- Nodos: 40-50
- Área: 1000m x 1000m
- Objetivo: Evaluar escalabilidad

### Escenario 4: Tráfico Pesado
- Rate: 4096 bps o superior
- Múltiples flujos simultáneos
- Objetivo: Evaluar throughput

## Mejores Prácticas

1. **Ejecutar múltiples corridas** (10-30) para resultados estadísticamente significativos
2. **Usar semillas aleatorias diferentes** para cada corrida
3. **Documentar todos los parámetros** de simulación
4. **Comparar resultados** con literatura existente
5. **Validar resultados** con herramientas múltiples

## Referencias

- NS-3 MANET Examples: `/examples/routing/`
- OLSR RFC: https://tools.ietf.org/html/rfc3626
- BATMAN Documentation: https://www.open-mesh.org/

## Siguiente Paso

Continuar con el [Análisis Estocástico](ANALISIS_ESTOCASTICO.md) de los resultados.
