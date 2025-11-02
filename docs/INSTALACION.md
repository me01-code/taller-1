# Guía de Instalación - Taller Métodos Estocásticos

Esta guía detalla el proceso de instalación de todas las herramientas necesarias para el taller.

## Índice

1. [Requisitos del Sistema](#requisitos-del-sistema)
2. [Instalación de NS-3](#instalación-de-ns-3)
3. [Instalación de NetAnim](#instalación-de-netanim)
4. [Configuración del Entorno](#configuración-del-entorno)
5. [Verificación de la Instalación](#verificación-de-la-instalación)
6. [Solución de Problemas](#solución-de-problemas)

## Requisitos del Sistema

### Hardware Mínimo
- Procesador: Dual-core 2.0 GHz o superior
- RAM: 4 GB mínimo (8 GB recomendado)
- Disco: 10 GB de espacio libre
- Gráficos: Soporte para OpenGL (para NetAnim)

### Software Base
- Sistema Operativo: Ubuntu 20.04 LTS o superior
- Conexión a Internet (para descargar dependencias)

## Instalación de NS-3

### Paso 1: Actualizar el Sistema

```bash
sudo apt-get update
sudo apt-get upgrade -y
```

### Paso 2: Instalar Dependencias Básicas

```bash
sudo apt-get install -y \
    g++ \
    python3 \
    python3-dev \
    python3-pip \
    cmake \
    ninja-build \
    git \
    ccache
```

### Paso 3: Instalar Dependencias de NS-3

```bash
# Bibliotecas esenciales
sudo apt-get install -y \
    pkg-config \
    sqlite3 \
    libsqlite3-dev \
    libxml2 \
    libxml2-dev \
    libboost-all-dev \
    gsl-bin \
    libgsl-dev

# Bibliotecas para visualización
sudo apt-get install -y \
    libgtk-3-dev \
    qtbase5-dev \
    qtchooser \
    qt5-qmake \
    qtbase5-dev-tools

# Bibliotecas opcionales pero recomendadas
sudo apt-get install -y \
    python3-setuptools \
    python3-pygraphviz \
    python3-kiwi \
    python3-pygoocanvas \
    tcpdump \
    wireshark
```

### Paso 4: Descargar NS-3

```bash
# Crear directorio de trabajo
cd ~
mkdir -p ns3-workspace
cd ns3-workspace

# Clonar repositorio NS-3
git clone https://gitlab.com/nsnam/ns-3-dev.git ns-3
cd ns-3

# Alternativamente, descargar una versión estable específica
# wget https://www.nsnam.org/release/ns-allinone-3.36.tar.bz2
# tar xjf ns-allinone-3.36.tar.bz2
```

### Paso 5: Configurar NS-3

```bash
cd ~/ns3-workspace/ns-3

# Configuración básica
./ns3 configure --enable-examples --enable-tests

# Configuración con más opciones (recomendado)
./ns3 configure \
    --enable-examples \
    --enable-tests \
    --enable-sudo \
    --with-python=/usr/bin/python3
```

### Paso 6: Compilar NS-3

```bash
# Compilar (puede tardar 10-30 minutos)
./ns3 build

# Verificar compilación
./test.py -c core
```

## Instalación de NetAnim

### Método 1: Desde el Código Fuente de NS-3

```bash
cd ~/ns3-workspace/ns-3/netanim

# Compilar NetAnim
qmake NetAnim.pro
make

# Ejecutar NetAnim (prueba)
./NetAnim
```

### Método 2: Instalación Independiente

```bash
# Clonar repositorio NetAnim
cd ~/ns3-workspace
git clone https://gitlab.com/nsnam/netanim.git
cd netanim

# Compilar
qmake NetAnim.pro
make
```

## Configuración del Entorno

### Variables de Entorno

Agregar al archivo `~/.bashrc`:

```bash
# NS-3 Environment
export NS3_HOME="$HOME/ns3-workspace/ns-3"
export PATH="$NS3_HOME:$PATH"
export PYTHONPATH="$NS3_HOME/build/bindings/python:$PYTHONPATH"

# Alias útiles
alias ns3='$NS3_HOME/ns3'
alias netanim='$NS3_HOME/netanim/NetAnim'
```

Luego recargar:

```bash
source ~/.bashrc
```

### Configuración de Python

```bash
# Instalar bibliotecas Python útiles
pip3 install numpy matplotlib pandas scipy
```

## Verificación de la Instalación

### Verificar NS-3

```bash
cd ~/ns3-workspace/ns-3

# Test rápido
./ns3 run hello-simulator

# Debería mostrar: "Hello Simulator"
```

### Verificar Módulos MANET

```bash
# Verificar que OLSR está disponible
./ns3 run "scratch-simulator --PrintHelp" | grep -i olsr

# Verificar ejemplos MANET
ls examples/routing/
```

### Verificar NetAnim

```bash
# Ejecutar NetAnim
cd ~/ns3-workspace/ns-3/netanim
./NetAnim

# Debería abrir la interfaz gráfica
```

### Test Completo

```bash
cd ~/ns3-workspace/ns-3

# Ejecutar ejemplo con animación
./ns3 run "wifi-adhoc --tracing=1"

# Debería generar archivo .xml para NetAnim
ls *.xml
```

## Solución de Problemas

### Error: "No se encuentra qmake"

```bash
# Instalar Qt5
sudo apt-get install qt5-default qtbase5-dev
```

### Error: "Cannot find Python.h"

```bash
# Instalar python3-dev
sudo apt-get install python3-dev
```

### Error: "libxml2 not found"

```bash
sudo apt-get install libxml2-dev
```

### Error de Compilación con ccache

```bash
# Limpiar y recompilar
./ns3 clean
./ns3 configure --enable-examples
./ns3 build
```

### NetAnim no Muestra la Animación

```bash
# Verificar que el archivo XML se generó
ls -lh *.xml

# Verificar permisos
chmod +x netanim/NetAnim

# Reinstalar bibliotecas Qt
sudo apt-get install --reinstall qtbase5-dev
```

### Problemas de Rendimiento en la Compilación

```bash
# Usar menos cores para compilar
./ns3 build -j2

# O especificar el número de trabajos
./ns3 build -j4
```

## Verificación Final

Ejecutar este script de verificación:

```bash
#!/bin/bash

echo "=== Verificación de Instalación NS-3 ==="

# Verificar NS-3
if [ -d "$HOME/ns3-workspace/ns-3" ]; then
    echo "✓ Directorio NS-3 encontrado"
else
    echo "✗ Directorio NS-3 no encontrado"
fi

# Verificar ejecutable
if [ -f "$HOME/ns3-workspace/ns-3/ns3" ]; then
    echo "✓ Ejecutable ns3 encontrado"
else
    echo "✗ Ejecutable ns3 no encontrado"
fi

# Verificar NetAnim
if [ -f "$HOME/ns3-workspace/ns-3/netanim/NetAnim" ]; then
    echo "✓ NetAnim encontrado"
else
    echo "✗ NetAnim no encontrado"
fi

# Verificar Python
if python3 --version > /dev/null 2>&1; then
    echo "✓ Python3 instalado: $(python3 --version)"
else
    echo "✗ Python3 no encontrado"
fi

echo "=== Verificación Completa ==="
```

## Recursos Adicionales

- [Documentación Oficial NS-3](https://www.nsnam.org/documentation/)
- [Tutorial NS-3](https://www.nsnam.org/docs/tutorial/html/)
- [NetAnim Wiki](https://www.nsnam.org/wiki/NetAnim)
- [Foro NS-3](https://groups.google.com/g/ns-3-users)

## Siguiente Paso

Una vez completada la instalación, continuar con la [Guía de Simulaciones](SIMULACIONES.md).
