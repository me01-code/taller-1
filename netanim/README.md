# Archivos NetAnim

Este directorio está destinado para almacenar archivos de animación XML generados por NS-3 para visualización con NetAnim.

## Contenido

Los archivos `.xml` generados por las simulaciones NS-3 se guardan aquí para su visualización posterior con NetAnim.

## Estructura Típica

```
netanim/
├── *.xml               # Archivos de animación
├── configs/            # Configuraciones personalizadas
└── screenshots/        # Capturas de pantalla
```

## Uso

### Generar Archivo de Animación

En tu script de simulación NS-3, incluir:

```cpp
#include "ns3/netanim-module.h"

// Crear animación
AnimationInterface anim("netanim/my-simulation.xml");
anim.EnablePacketMetadata(true);
```

### Abrir con NetAnim

```bash
cd ~/ns3-workspace/ns-3/netanim
./NetAnim

# Luego: File → Open → Seleccionar archivo XML
```

## Gestión de Archivos

### Compresión

Los archivos XML pueden ser muy grandes. Comprimir después de generar:

```bash
# Comprimir archivo individual
gzip netanim/simulation.xml

# Descomprimir cuando sea necesario
gunzip netanim/simulation.xml.gz

# Comprimir todos los archivos grandes
find netanim/ -name "*.xml" -size +10M -exec gzip {} \;
```

### Limpieza

```bash
# Eliminar archivos antiguos
find netanim/ -name "*.xml" -mtime +30 -delete

# Mantener solo los más recientes
ls -t netanim/*.xml | tail -n +6 | xargs rm
```

## Tipos de Archivos

### Archivos de Animación (.xml)

Contienen:
- Posiciones de nodos en el tiempo
- Transmisiones de paquetes
- Metadata de red

### Archivos de Configuración

Configuraciones guardadas de NetAnim para reutilizar visualizaciones.

### Capturas de Pantalla

Imágenes exportadas desde NetAnim.

## Mejores Prácticas

1. **Nomenclatura**: Usar nombres descriptivos
   ```
   protocol_nodes_scenario_date.xml
   Ejemplo: olsr_20n_highmobility_20251102.xml
   ```

2. **Tamaño**: Controlar tamaño de archivos
   - Desactivar metadata si no es necesaria
   - Limitar número de paquetes trazados
   - Comprimir archivos grandes

3. **Organización**: Subdirectorios por experimento
   ```
   netanim/
   ├── experiment1/
   │   ├── olsr.xml
   │   └── batman.xml
   └── experiment2/
       └── comparison.xml
   ```

4. **Documentación**: Incluir README en subdirectorios
   - Descripción del experimento
   - Parámetros usados
   - Fecha de generación

## Solución de Problemas

### Archivo muy Grande

```bash
# Ver tamaño
ls -lh netanim/*.xml

# Si es > 100MB, considerar:
# 1. Reducir duración de simulación
# 2. Desactivar metadata
# 3. Aumentar intervalo de actualización
```

### NetAnim no Abre el Archivo

```bash
# Verificar formato XML
xmllint --noout netanim/simulation.xml

# Si hay errores, el archivo puede estar corrupto
# Regenerar simulación
```

### Sin Espacio en Disco

```bash
# Limpiar archivos antiguos
rm netanim/old_*.xml

# O comprimir todos
gzip netanim/*.xml
```

## Configuración Recomendada

Para diferentes tipos de simulaciones:

### Simulación Corta (< 1 min)

```cpp
AnimationInterface anim("netanim/short-sim.xml");
anim.EnablePacketMetadata(true);  // Todo el detalle
anim.SetMaxPktsPerTraceFile(500000);
```

### Simulación Media (1-5 min)

```cpp
AnimationInterface anim("netanim/medium-sim.xml");
anim.EnablePacketMetadata(false);  // Sin metadata
anim.SetMobilityPollInterval(Seconds(0.5));
```

### Simulación Larga (> 5 min)

```cpp
AnimationInterface anim("netanim/long-sim.xml");
anim.EnablePacketMetadata(false);
anim.SetMobilityPollInterval(Seconds(1.0));
anim.SetMaxPktsPerTraceFile(100000);
```

## Referencias

- [Documentación NetAnim](https://www.nsnam.org/wiki/NetAnim)
- [NS-3 Animation Guide](https://www.nsnam.org/docs/models/html/animation.html)
- Ver también: `docs/NETANIM.md`

## Notas

⚠️ **Importante**: Los archivos XML pueden ocupar mucho espacio. No versionar en Git archivos > 10MB.

Agregar al `.gitignore`:
```
netanim/*.xml
netanim/**/*.xml
!netanim/example-small.xml  # Excepto ejemplos pequeños
```
