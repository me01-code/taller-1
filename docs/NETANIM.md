# Guía de NetAnim

NetAnim es la herramienta de visualización y animación para NS-3 que permite observar el comportamiento de las simulaciones de red en tiempo real.

## Índice

1. [Introducción](#introducción)
2. [Instalación](#instalación)
3. [Generar Archivos de Animación](#generar-archivos-de-animación)
4. [Uso de NetAnim](#uso-de-netanim)
5. [Personalización](#personalización)
6. [Interpretación de Resultados](#interpretación-de-resultados)
7. [Tips y Trucos](#tips-y-trucos)

## Introducción

NetAnim permite:
- Visualizar movimiento de nodos
- Observar transmisión de paquetes
- Analizar topología de red
- Generar estadísticas visuales
- Crear videos de simulaciones

### Características Principales

- **Visualización 2D**: Representación clara de nodos y enlaces
- **Reproducción controlada**: Play, pause, avance rápido/lento
- **Estadísticas en tiempo real**: Paquetes transmitidos, recibidos, perdidos
- **Trayectorias de nodos**: Historial de movimiento
- **Exportación**: Guardar imágenes y videos

## Instalación

### Método 1: Desde NS-3

```bash
cd ~/ns3-workspace/ns-3/netanim
qmake NetAnim.pro
make

# Ejecutar
./NetAnim
```

### Método 2: Instalación Independiente

```bash
# Instalar dependencias Qt5
sudo apt-get install qt5-default qtbase5-dev

# Clonar repositorio
cd ~/ns3-workspace
git clone https://gitlab.com/nsnam/netanim.git
cd netanim

# Compilar
qmake NetAnim.pro
make
```

### Verificar Instalación

```bash
./NetAnim --version
```

## Generar Archivos de Animación

### En Código C++

Para habilitar NetAnim en tus simulaciones NS-3:

```cpp
#include "ns3/netanim-module.h"

// Al final de la configuración, antes de Simulator::Run()
AnimationInterface anim("mi-animacion.xml");

// Opcional: Personalizar visualización
anim.EnablePacketMetadata(true);
anim.SetMaxPktsPerTraceFile(500000);

// Personalizar nodos
for (uint32_t i = 0; i < nodes.GetN(); ++i)
{
    anim.UpdateNodeDescription(nodes.Get(i), "Nodo-" + std::to_string(i));
    anim.UpdateNodeColor(nodes.Get(i), 0, 255, 0);  // Verde
    anim.UpdateNodeSize(i, 5.0, 5.0);  // Tamaño en NS
}

// Marcar nodo especial (ej. servidor)
anim.UpdateNodeColor(nodes.Get(0), 255, 0, 0);  // Rojo
anim.UpdateNodeDescription(nodes.Get(0), "Servidor");

Simulator::Run();
```

### Opciones de Configuración

```cpp
// Habilitar metadata de paquetes (más detalle, archivo más grande)
anim.EnablePacketMetadata(true);

// Establecer límite de paquetes por archivo
anim.SetMaxPktsPerTraceFile(500000);

// Configurar actualización de movilidad
anim.SetMobilityPollInterval(Seconds(1.0));

// Habilitar trazado de IP
anim.EnableIpv4RouteTracking("route-table.xml", 
                             Seconds(0), 
                             Seconds(simTime), 
                             Seconds(0.25));

// Establecer constantes de transmisión
anim.SetConstantPosition(nodes.Get(0), 10.0, 10.0);
```

### Ejemplo Completo

```cpp
#include "ns3/netanim-module.h"

// Crear animación
std::string animFile = "manet-simulation.xml";
AnimationInterface anim(animFile);

// Habilitar metadata
anim.EnablePacketMetadata(true);

// Personalizar apariencia de nodos
for (uint32_t i = 0; i < nodes.GetN(); ++i)
{
    // Descripción
    std::ostringstream oss;
    oss << "Node_" << i;
    anim.UpdateNodeDescription(nodes.Get(i), oss.str());
    
    // Color (verde para nodos normales)
    anim.UpdateNodeColor(nodes.Get(i), 0, 255, 0);
    
    // Tamaño
    anim.UpdateNodeSize(i, 10.0, 10.0);
}

// Servidor en rojo
anim.UpdateNodeColor(nodes.Get(0), 255, 0, 0);
anim.UpdateNodeDescription(nodes.Get(0), "Server");

// Clientes en azul
for (uint32_t i = 1; i < 5; ++i)
{
    anim.UpdateNodeColor(nodes.Get(i), 0, 0, 255);
}

std::cout << "Archivo de animación: " << animFile << std::endl;
```

## Uso de NetAnim

### Abrir NetAnim

```bash
cd ~/ns3-workspace/ns-3/netanim
./NetAnim
```

### Cargar Archivo de Animación

1. **File → Open XML trace file**
2. Navegar al archivo `.xml` generado
3. Seleccionar y abrir

### Controles Básicos

#### Panel de Control

- **Play/Pause**: Iniciar/pausar animación
- **Stop**: Detener y reiniciar
- **Slider**: Navegar a tiempo específico
- **Speed**: Ajustar velocidad de reproducción

#### Zoom y Navegación

- **Mouse Wheel**: Zoom in/out
- **Click + Drag**: Mover vista
- **Fit to Window**: Ajustar vista a ventana

### Paneles de Información

#### 1. Node Positions
Muestra coordenadas X, Y de cada nodo

#### 2. Packet Statistics
- Paquetes transmitidos
- Paquetes recibidos
- Paquetes en tránsito

#### 3. Node/Link Statistics
- Tráfico por nodo
- Tráfico por enlace

#### 4. Packet Timeline
Línea de tiempo de transmisiones

## Personalización

### Configurar Vista

#### Ajustar Grid

```
View → Show Grid
View → Grid Lines → [Spacing]
```

#### Mostrar IDs de Nodos

```
View → Show Node IDs
```

#### Mostrar Trayectorias

```
View → Show Node Trajectory
```

### Colores y Etiquetas

#### En Tiempo de Ejecución

- Click derecho en nodo → Properties
- Cambiar color, tamaño, etiqueta

#### En Código

```cpp
// Esquema de colores por rol
void ColorizeNodes(NodeContainer nodes, AnimationInterface &anim)
{
    // Servidor - Rojo
    anim.UpdateNodeColor(nodes.Get(0), 255, 0, 0);
    
    // Routers - Amarillo
    for (uint32_t i = 1; i <= 5; ++i)
    {
        anim.UpdateNodeColor(nodes.Get(i), 255, 255, 0);
    }
    
    // Clientes - Azul
    for (uint32_t i = 6; i < nodes.GetN(); ++i)
    {
        anim.UpdateNodeColor(nodes.Get(i), 0, 0, 255);
    }
}
```

### Filtros de Paquetes

```
Packet → Filter by Protocol
Packet → Filter by Source/Destination
```

### Exportar Visualizaciones

#### Capturar Imagen

```
File → Save Screenshot
```

#### Exportar Video

```bash
# Instalar herramientas
sudo apt-get install ffmpeg

# Capturar frames
# (usar NetAnim para avanzar y guardar screenshots)

# Crear video desde frames
ffmpeg -framerate 10 -i frame_%04d.png -c:v libx264 \
       -pix_fmt yuv420p simulation.mp4
```

## Interpretación de Resultados

### Visualización de Movilidad

#### Patrones a Observar

1. **Distribución Espacial**
   - Nodos agrupados vs dispersos
   - Áreas de alta/baja densidad

2. **Trayectorias**
   - Movimiento aleatorio vs dirigido
   - Velocidades variables
   - Patrones de pausa

3. **Conectividad**
   - Enlaces establecidos/rotos
   - Particiones de red
   - Componentes conectados

### Transmisión de Paquetes

#### Interpretación Visual

- **Líneas Verdes**: Transmisión exitosa
- **Líneas Rojas**: Paquete perdido/colisión
- **Líneas Amarillas**: Paquete en tránsito

#### Métricas

```
Statistics → Packet Stats
- Total Packets: Total transmitido
- Received: Entregados exitosamente
- Dropped: Perdidos
```

### Análisis de Protocolos

#### OLSR

**Observar:**
- Mensajes HELLO periódicos (círculos pequeños)
- Mensajes TC de topología (círculos medianos)
- Selección de MPRs (nodos con más conexiones)

#### BATMAN

**Observar:**
- OGMs (Originator Messages) periódicos
- Reenvío de OGMs por nodos intermedios
- Adaptación rápida a cambios de topología

### Detección de Problemas

#### Particiones de Red

Si observas grupos de nodos sin comunicación:
- Red fragmentada
- Nodos fuera de rango
- Protocolo no converge

#### Congestión

Si ves muchas líneas rojas:
- Alta pérdida de paquetes
- Colisiones frecuentes
- Sobrecarga de red

#### Rutas Subóptimas

Si los paquetes toman caminos largos:
- Protocolo no encuentra ruta óptima
- Información de topología desactualizada
- Alta movilidad

## Tips y Trucos

### Rendimiento

```cpp
// Para simulaciones grandes, reducir metadata
anim.EnablePacketMetadata(false);

// Limitar paquetes por archivo
anim.SetMaxPktsPerTraceFile(100000);

// Aumentar intervalo de actualización de movilidad
anim.SetMobilityPollInterval(Seconds(1.0));
```

### Depuración

```cpp
// Agregar contadores en nodos
uint32_t nodeId = 0;
anim.UpdateNodeCounter(nodeId, "PacketsSent", packetCount);

// Actualizar descripción dinámicamente
std::ostringstream desc;
desc << "Node_" << nodeId << " [" << packetCount << " pkts]";
anim.UpdateNodeDescription(nodes.Get(nodeId), desc.str());
```

### Comparación de Escenarios

1. Generar animaciones de diferentes configuraciones:
   - `scenario1.xml`
   - `scenario2.xml`
   - `scenario3.xml`

2. Abrir en ventanas separadas de NetAnim

3. Reproducir simultáneamente para comparar

### Scripts para Procesamiento

```python
# Extraer información de XML
import xml.etree.ElementTree as ET

tree = ET.parse('simulation.xml')
root = tree.getroot()

# Contar paquetes
packets = root.findall('.//p')
print(f"Total de paquetes: {len(packets)}")

# Analizar posiciones de nodos
for node in root.findall('.//node'):
    node_id = node.get('id')
    x = node.get('x')
    y = node.get('y')
    print(f"Nodo {node_id}: ({x}, {y})")
```

### Mejores Prácticas

1. **Archivos Pequeños**: Para animaciones largas, limitar metadata
2. **Colores Significativos**: Usar colores consistentes para roles
3. **Etiquetas Claras**: Nombres descriptivos para nodos
4. **Documentar**: Incluir descripción en código
5. **Backup**: Guardar archivos XML (pueden ser grandes)

## Solución de Problemas

### NetAnim no Abre el Archivo

```bash
# Verificar que el archivo existe y no está vacío
ls -lh simulation.xml

# Verificar formato XML
xmllint --noout simulation.xml
```

### Animación muy Lenta

- Reducir metadata: `EnablePacketMetadata(false)`
- Aumentar intervalo de movilidad
- Limitar número de paquetes

### Nodos no se Mueven

- Verificar que se instaló modelo de movilidad
- Verificar que `MobilityPollInterval` no es muy largo
- Asegurar que simulación se ejecutó completamente

### Colores no Aparecen

- Actualizar color después de instalar movilidad
- Usar valores RGB válidos (0-255)
- Verificar que se llama antes de `Simulator::Run()`

## Recursos Adicionales

- [NetAnim Wiki](https://www.nsnam.org/wiki/NetAnim)
- [NetAnim Tutorial Video](https://www.youtube.com/results?search_query=netanim+tutorial)
- [NS-3 Animation Examples](https://www.nsnam.org/docs/models/html/animation.html)

## Conclusión

NetAnim es una herramienta poderosa para:
- Entender comportamiento de red
- Depurar simulaciones
- Presentar resultados visualmente
- Validar configuraciones

La visualización ayuda a identificar problemas que no son evidentes en estadísticas numéricas.
