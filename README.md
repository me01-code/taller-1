# Taller 1 - Métodos Estocásticos: Redes MANET

Este repositorio contiene el material del Taller 1 de la asignatura de Métodos Estocásticos, enfocado en la simulación y análisis de redes móviles ad-hoc (MANET) utilizando NS-3.

## Descripción

Este taller explora protocolos de enrutamiento en redes MANET mediante simulaciones con NS-3 (Network Simulator 3), visualización con NetAnim, y análisis de comportamiento estocástico en redes móviles.

### Protocolos Cubiertos

- **OLSR** (Optimized Link State Routing): Protocolo proactivo de enrutamiento para redes ad-hoc
- **BATMAN** (Better Approach To Mobile Adhoc Networking): Protocolo de enrutamiento descentralizado

## Contenido del Repositorio

```
taller-1/
├── docs/              # Documentación del taller
├── scripts/           # Scripts de simulación NS-3
├── examples/          # Ejemplos de configuración
├── results/           # Resultados de simulaciones
└── netanim/          # Configuraciones NetAnim
```

## Requisitos Previos

### Software Necesario

- **NS-3** (versión 3.36 o superior)
- **NetAnim** (Network Animator)
- **Python 3** (para scripts de análisis)
- **GCC/G++** (para compilar NS-3)
- **CMake** (versión 3.10 o superior)

### Sistema Operativo

- Ubuntu 20.04 o superior (recomendado)
- Debian 10 o superior
- Otras distribuciones Linux (con ajustes)

## Instalación

### 1. Instalar Dependencias

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y g++ python3 cmake ninja-build git

# Dependencias adicionales para NS-3
sudo apt-get install -y python3-dev pkg-config sqlite3 \
    libsqlite3-dev libxml2 libxml2-dev libgtk-3-dev \
    qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools
```

### 2. Descargar e Instalar NS-3

```bash
# Clonar NS-3
cd ~
git clone https://gitlab.com/nsnam/ns-3-dev.git ns-3
cd ns-3

# Configurar y compilar
./ns3 configure --enable-examples --enable-tests
./ns3 build
```

### 3. Instalar NetAnim

```bash
# Descargar NetAnim
cd ~/ns-3/netanim
qmake NetAnim.pro
make
```

### 4. Clonar este Repositorio

```bash
cd ~
git clone https://github.com/me01-code/taller-1.git
cd taller-1
```

## Uso

### Ejecutar Simulaciones

Los scripts de simulación se encuentran en el directorio `scripts/`. Para ejecutarlos:

```bash
# Copiar scripts al directorio de NS-3
cp scripts/*.cc ~/ns-3/scratch/

# Compilar y ejecutar
cd ~/ns-3
./ns3 run scratch/manet-olsr
./ns3 run scratch/manet-batman
```

### Visualización con NetAnim

```bash
# Las simulaciones generan archivos .xml para NetAnim
cd ~/ns-3/netanim
./NetAnim
# Abrir el archivo XML generado en results/
```

### Análisis de Resultados

```bash
# Scripts Python para análisis estadístico
cd ~/taller-1
python3 scripts/analyze_results.py
```

## Estructura de Simulaciones

### Topología de Red

Las simulaciones utilizan:
- Número de nodos: 10-50 nodos móviles
- Área de simulación: 1000m x 1000m
- Modelos de movilidad: Random Waypoint, Random Walk
- Patrones de tráfico: CBR (Constant Bit Rate)

### Métricas Evaluadas

1. **Throughput**: Tasa de transferencia de datos
2. **Delay**: Retardo end-to-end
3. **Packet Delivery Ratio (PDR)**: Razón de entrega de paquetes
4. **Overhead**: Sobrecarga de control del protocolo
5. **Energy Consumption**: Consumo energético estimado

## Ejercicios del Taller

### Ejercicio 1: Comparación OLSR vs BATMAN
Comparar el desempeño de ambos protocolos en diferentes escenarios de movilidad.

### Ejercicio 2: Análisis Estocástico
Analizar la variabilidad y distribuciones de probabilidad de las métricas de red.

### Ejercicio 3: Optimización de Parámetros
Optimizar parámetros de protocolo para diferentes condiciones de red.

## Referencias

- [NS-3 Documentation](https://www.nsnam.org/documentation/)
- [OLSR RFC 3626](https://tools.ietf.org/html/rfc3626)
- [BATMAN Protocol](https://www.open-mesh.org/projects/batman-adv/wiki)
- [NetAnim Tutorial](https://www.nsnam.org/wiki/NetAnim)

## Contribuciones

Este es un proyecto académico. Para contribuir:

1. Fork el repositorio
2. Crear una rama para tu característica
3. Commit tus cambios
4. Push a la rama
5. Crear un Pull Request

## Licencia

MIT License - ver archivo [LICENSE](LICENSE) para más detalles.

## Autores

- Asignatura: Métodos Estocásticos
- Institución: Universidad Nacional de Colombia

## Contacto

Para preguntas sobre el taller, contactar al profesor de la asignatura.
