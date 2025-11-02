# Resumen del Repositorio Taller-1

## Descripción General

Este repositorio contiene el material completo para el **Taller 1 de Métodos Estocásticos** enfocado en redes MANET (Mobile Ad-hoc NETworks), utilizando NS-3 (Network Simulator 3) para simulaciones.

## Estructura del Repositorio

```
taller-1/
│
├── README.md                      # Documentación principal del proyecto
├── LICENSE                        # Licencia MIT
├── SUMMARY.md                     # Este archivo - resumen general
│
├── docs/                          # 📚 Documentación técnica completa
│   ├── README.md                  # Índice de documentación
│   ├── INSTALACION.md             # Guía de instalación de NS-3 y NetAnim
│   ├── SIMULACIONES.md            # Guía de simulaciones MANET
│   ├── ANALISIS_ESTOCASTICO.md    # Métodos estocásticos aplicados
│   └── NETANIM.md                 # Guía de uso de NetAnim
│
├── scripts/                       # 💻 Scripts de simulación y análisis
│   ├── README.md                  # Guía de uso de scripts
│   ├── manet-olsr.cc              # Simulación NS-3 con protocolo OLSR
│   └── analyze_results.py         # Análisis estadístico de resultados
│
├── examples/                      # 📋 Ejemplos y configuraciones
│   ├── README.md                  # Guía de ejemplos
│   ├── simulation_params.json     # Parámetros de configuración
│   └── sample_results.csv         # Datos de ejemplo
│
├── results/                       # 📊 Directorio para resultados
│   └── README.md                  # Guía de gestión de resultados
│
└── netanim/                       # 🎬 Archivos de animación
    └── README.md                  # Guía de archivos NetAnim
```

## Contenido por Directorio

### 📚 Documentación (`docs/`)

**5 documentos principales** con más de 2,500 líneas de contenido:

1. **INSTALACION.md** (330 líneas)
   - Instalación paso a paso de NS-3
   - Configuración de NetAnim
   - Solución de problemas
   - Verificación de instalación

2. **SIMULACIONES.md** (362 líneas)
   - Protocolos OLSR y BATMAN
   - Configuración de simulaciones
   - Métricas de rendimiento
   - Análisis con FlowMonitor

3. **ANALISIS_ESTOCASTICO.md** (504 líneas)
   - Modelos de movilidad (Random Waypoint, Random Walk)
   - Distribuciones de probabilidad
   - Procesos estocásticos (Poisson, Markov)
   - Pruebas estadísticas
   - Análisis de correlación

4. **NETANIM.md** (477 líneas)
   - Uso de NetAnim
   - Personalización de visualizaciones
   - Interpretación de resultados
   - Tips y trucos

5. **README.md** (219 líneas)
   - Índice general de documentación
   - Flujo de trabajo recomendado
   - Objetivos de aprendizaje
   - Recursos adicionales

### 💻 Scripts (`scripts/`)

**Scripts de simulación NS-3:**

- **manet-olsr.cc** (400+ líneas)
  - Simulación completa MANET con OLSR
  - Configuración de WiFi ad-hoc
  - Modelos de movilidad
  - Aplicaciones CBR
  - FlowMonitor integrado
  - Soporte para NetAnim
  - Parámetros configurables por línea de comandos

**Scripts de análisis Python:**

- **analyze_results.py** (450+ líneas)
  - Estadísticas descriptivas
  - Intervalos de confianza
  - Pruebas de hipótesis (t-test, ANOVA)
  - Análisis de correlación
  - Ajuste de distribuciones
  - Generación automática de gráficos

### 📋 Ejemplos (`examples/`)

- **simulation_params.json**: Configuración completa en JSON
- **sample_results.csv**: Datos de ejemplo para análisis
- Escenarios predefinidos (baja/alta movilidad, alta densidad, etc.)

### 📊 Resultados (`results/`)

Directorio organizado para almacenar:
- Datos crudos de simulaciones
- Resultados procesados
- Gráficos generados
- Animaciones NetAnim
- Informes técnicos

### 🎬 NetAnim (`netanim/`)

Directorio para archivos de animación XML generados por NS-3.

## Características Principales

### ✅ Documentación Completa

- **2,500+ líneas** de documentación técnica
- Cobertura de todos los aspectos del taller
- Guías paso a paso
- Ejemplos prácticos
- Solución de problemas

### ✅ Código Funcional

- Script NS-3 completo para OLSR
- Análisis estadístico en Python
- Soporte para NetAnim
- FlowMonitor integrado
- Parámetros configurables

### ✅ Ejemplos Prácticos

- Archivos de configuración JSON
- Datos de ejemplo CSV
- Escenarios predefinidos
- Scripts de batch

### ✅ Herramientas de Análisis

- Estadísticas descriptivas
- Pruebas de hipótesis
- Análisis de correlación
- Visualización de datos
- Ajuste de distribuciones

## Tecnologías Cubiertas

### Simulación
- **NS-3**: Network Simulator 3
- **NetAnim**: Network Animator
- **OLSR**: Optimized Link State Routing
- **BATMAN**: Better Approach To Mobile Adhoc Networking

### Análisis
- **Python**: NumPy, Pandas, Matplotlib, SciPy
- **Estadística**: Distribuciones, intervalos de confianza, hipótesis
- **Visualización**: Histogramas, scatter plots, boxplots

### Métodos Estocásticos
- Modelos de movilidad aleatoria
- Procesos de Poisson
- Cadenas de Markov
- Distribuciones log-normal, exponencial, binomial
- Análisis de correlación y regresión

## Casos de Uso

### 1. Instalación y Configuración
```bash
# Seguir docs/INSTALACION.md
sudo apt-get install ns-3-dependencies
git clone ns-3-repository
./ns3 configure && ./ns3 build
```

### 2. Ejecutar Simulación
```bash
# Copiar script a NS-3
cp scripts/manet-olsr.cc ~/ns3/scratch/

# Ejecutar
cd ~/ns3
./ns3 run "scratch/manet-olsr --nodes=20 --duration=100"
```

### 3. Visualizar con NetAnim
```bash
# Ejecutar con NetAnim habilitado
./ns3 run "scratch/manet-olsr --netanim=1"

# Abrir NetAnim y cargar XML
~/ns3/netanim/NetAnim
```

### 4. Analizar Resultados
```bash
# Análisis estadístico
python3 scripts/analyze_results.py results/data.csv
```

## Objetivos Cumplidos

✅ Estructura completa del repositorio  
✅ Documentación exhaustiva (5 documentos)  
✅ Scripts de simulación NS-3 funcionales  
✅ Scripts de análisis Python completos  
✅ Ejemplos y configuraciones  
✅ Guías de instalación y uso  
✅ Cobertura de métodos estocásticos  
✅ Soporte para NetAnim  
✅ Sistema de análisis estadístico  
✅ Recursos educativos completos  

## Métricas del Proyecto

| Categoría | Cantidad |
|-----------|----------|
| Documentos Markdown | 14 archivos |
| Líneas de Documentación | 2,500+ líneas |
| Scripts de Simulación | 1 (OLSR, ~400 líneas) |
| Scripts de Análisis | 1 (Python, ~450 líneas) |
| Archivos de Configuración | 2 (JSON, CSV) |
| Directorios | 6 principales |
| Idioma | Español |

## Próximos Pasos

Para usuarios del taller:

1. **Leer** `README.md` para visión general
2. **Instalar** siguiendo `docs/INSTALACION.md`
3. **Ejecutar** ejemplos básicos
4. **Simular** con `scripts/manet-olsr.cc`
5. **Analizar** con `scripts/analyze_results.py`
6. **Estudiar** `docs/ANALISIS_ESTOCASTICO.md`
7. **Visualizar** con NetAnim

## Expansión Futura

Posibles mejoras:
- [ ] Script de simulación BATMAN
- [ ] Script de comparación entre protocolos
- [ ] Más modelos de movilidad
- [ ] Análisis de machine learning
- [ ] Interfaz web para resultados
- [ ] Notebooks Jupyter interactivos
- [ ] Videos tutoriales
- [ ] Tests automatizados

## Contribuciones

Este es un proyecto educativo. Para contribuir:
1. Fork del repositorio
2. Crear branch de característica
3. Commit de cambios
4. Push y crear Pull Request

## Licencia

MIT License - Ver `LICENSE` para detalles

## Autores

- **Asignatura**: Métodos Estocásticos
- **Institución**: Universidad Nacional de Colombia
- **Año**: 2025

## Contacto

Para preguntas sobre el taller, contactar al profesor de la asignatura.

---

**Nota**: Este repositorio proporciona una base completa para aprender sobre redes MANET, simulación con NS-3, y análisis estocástico de redes móviles ad-hoc.

**Última actualización**: 2025-11-02  
**Versión**: 1.0
