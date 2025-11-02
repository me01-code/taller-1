# Documentación del Taller

Bienvenido a la documentación del Taller de Métodos Estocásticos aplicados a Redes MANET.

## Índice de Documentos

### 1. [INSTALACION.md](INSTALACION.md)
Guía completa de instalación de NS-3, NetAnim y todas las dependencias necesarias.

**Contenido:**
- Requisitos del sistema
- Instalación paso a paso de NS-3
- Instalación de NetAnim
- Configuración del entorno
- Verificación de la instalación
- Solución de problemas comunes

### 2. [SIMULACIONES.md](SIMULACIONES.md)
Guía práctica para ejecutar simulaciones MANET con protocolos OLSR y BATMAN.

**Contenido:**
- Introducción a redes MANET
- Descripción de protocolos OLSR y BATMAN
- Configuración de simulaciones
- Ejecución de simulaciones
- Métricas y análisis de resultados
- Visualización con NetAnim

### 3. [ANALISIS_ESTOCASTICO.md](ANALISIS_ESTOCASTICO.md)
Fundamentos teóricos y prácticos del análisis estocástico de redes MANET.

**Contenido:**
- Fundamentos de procesos estocásticos
- Modelos de movilidad
- Análisis de tráfico
- Distribuciones de probabilidad
- Pruebas estadísticas
- Análisis de correlación

### 4. [NETANIM.md](NETANIM.md)
Guía de uso de NetAnim para visualización de simulaciones.

**Contenido:**
- Introducción a NetAnim
- Configuración de animaciones
- Personalización de visualizaciones
- Interpretación de resultados
- Tips y trucos

## Flujo de Trabajo Recomendado

```
1. Instalación
   ↓
2. Verificación
   ↓
3. Ejecución de Ejemplos
   ↓
4. Simulaciones Propias
   ↓
5. Análisis de Resultados
   ↓
6. Informe Final
```

## Orden de Lectura

Para aprovechar al máximo este taller, recomendamos seguir este orden:

1. **Comenzar con**: [INSTALACION.md](INSTALACION.md)
   - Completar toda la instalación
   - Verificar que todo funcione correctamente

2. **Continuar con**: [SIMULACIONES.md](SIMULACIONES.md)
   - Entender los conceptos básicos de MANET
   - Aprender sobre OLSR y BATMAN
   - Ejecutar las primeras simulaciones

3. **Profundizar en**: [ANALISIS_ESTOCASTICO.md](ANALISIS_ESTOCASTICO.md)
   - Estudiar los fundamentos teóricos
   - Aplicar análisis estadístico a resultados
   - Realizar pruebas de hipótesis

4. **Complementar con**: [NETANIM.md](NETANIM.md)
   - Visualizar simulaciones
   - Interpretar animaciones
   - Crear presentaciones visuales

## Recursos Adicionales

### Ejemplos de Código

Todos los ejemplos de código están disponibles en:
- `/scripts/`: Scripts de simulación NS-3
- `/examples/`: Configuraciones y datos de ejemplo

### Archivos de Configuración

- `simulation_params.json`: Parámetros de simulación
- `sample_results.csv`: Formato de resultados

### Scripts de Análisis

- `analyze_results.py`: Análisis estadístico de resultados
- Requiere: NumPy, Pandas, Matplotlib, SciPy

## Objetivos de Aprendizaje

Al completar este taller, deberías ser capaz de:

✓ Instalar y configurar NS-3 y NetAnim  
✓ Ejecutar simulaciones de redes MANET  
✓ Comparar protocolos OLSR y BATMAN  
✓ Analizar resultados con métodos estocásticos  
✓ Aplicar pruebas estadísticas  
✓ Interpretar distribuciones de probabilidad  
✓ Visualizar simulaciones con NetAnim  
✓ Generar informes técnicos con resultados

## Evaluación

### Entregables

1. **Simulaciones**: Ejecutar y documentar al menos 3 escenarios diferentes
2. **Análisis**: Comparación estadística entre OLSR y BATMAN
3. **Informe**: Documento técnico con resultados y conclusiones
4. **Presentación**: Diapositivas con visualizaciones y hallazgos

### Criterios de Evaluación

- Correcta ejecución de simulaciones (25%)
- Análisis estadístico apropiado (25%)
- Interpretación de resultados (25%)
- Calidad del informe técnico (25%)

## Preguntas Frecuentes

### ¿Qué sistema operativo debo usar?

Se recomienda Ubuntu 20.04 LTS o superior. También funciona en Debian 10+ y otras distribuciones Linux con ajustes.

### ¿Cuánto espacio en disco necesito?

Aproximadamente 10 GB para NS-3, NetAnim y todas las dependencias.

### ¿Cuánto tiempo toma compilar NS-3?

Entre 10 y 30 minutos dependiendo de tu hardware.

### ¿Puedo usar Windows?

Es posible usando WSL2 (Windows Subsystem for Linux), pero se recomienda Linux nativo para mejor rendimiento.

### ¿Necesito conocimientos previos?

Es útil tener conocimientos básicos de:
- Programación en C++ (para modificar simulaciones)
- Python (para análisis de datos)
- Redes de computadoras
- Estadística básica

### ¿Dónde puedo obtener ayuda?

1. Documentación oficial de NS-3: https://www.nsnam.org/
2. Foro de NS-3: https://groups.google.com/g/ns-3-users
3. Stack Overflow con tag [ns-3]
4. Profesor del curso

## Recursos en Línea

### Documentación Oficial

- [NS-3 Documentation](https://www.nsnam.org/documentation/)
- [NS-3 Tutorial](https://www.nsnam.org/docs/tutorial/html/)
- [NetAnim Wiki](https://www.nsnam.org/wiki/NetAnim)

### Artículos y Papers

- OLSR RFC: https://tools.ietf.org/html/rfc3626
- BATMAN Documentation: https://www.open-mesh.org/
- Survey on MANET routing protocols

### Videos y Tutoriales

- NS-3 Tutorial Videos
- NetAnim Tutorial
- MANET Protocol Explanations

### Libros Recomendados

1. "Mobile Ad Hoc Networking" - Stefano Basagni et al.
2. "Network Simulations with NS-3" - Jafar Ababneh
3. "Introduction to Probability Models" - Sheldon Ross

## Contribuciones

Este es un proyecto académico en continuo desarrollo. Las contribuciones son bienvenidas:

1. Fork del repositorio
2. Crear branch para tu mejora
3. Commit de cambios
4. Push al branch
5. Crear Pull Request

## Licencia

Este material está bajo licencia MIT. Ver [LICENSE](../LICENSE) para más detalles.

## Contacto

Para preguntas sobre el taller:
- Profesor del curso
- Institución: Universidad Nacional de Colombia
- Asignatura: Métodos Estocásticos

---

**Última actualización**: 2025-11-02  
**Versión**: 1.0
