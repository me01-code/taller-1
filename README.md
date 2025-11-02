# MANET Jerárquica (Op A) - manet-jerarquica.cc

Descripción
- Implementación de una MANET jerárquica (Opción A). Líderes patrullan con movilidad dinámica; subordinados se mueven alrededor del líder.
- Archivo principal: [manet-jerarquica.cc](manet-jerarquica.cc).
- Modelo de movilidad de líderes: [`LeaderTrajectoryMobilityModel`](manet-jerarquica.cc).
- Funciones relevantes: [`SetupInterClusterTraffic`](manet-jerarquica.cc), [`ScheduleConnectivityMonitoring`](manet-jerarquica.cc).

Requisitos
- ns-3 versión usada: ns-3.46.1
- ns-3 debe estar compilado previamente.

Instalación / uso
1. Coloca la carpeta `taller-1` dentro de `scratch` del árbol de ns-3 (o haz clone del repositorio en `scratch/taller-1`).
2. Asegúrate de haber compilado ns-3 (build).
3. Ejecuta la simulación con:
   `./ns3 run scratch/taller-1/manet-jerarquica.cc`

Archivos de salida
- Animación NetAnim: `manet-jerarquica/manet-jerarquica-opA.xml`
- Rutas NetAnim: `manet-jerarquica/manet-jerarquica-opA-routes.xml`

Notas
- Ajusta parámetros de simulación dentro de `manet-jerarquica.cc` (p. ej. duración, velocidad de líderes, configuración de clusters).
- Para inspeccionar la implementación principal y modelos, abre [manet-jerarquica.cc](manet-jerarquica.cc).

Licencia
- Este repositorio incluye una licencia MIT en el archivo `LICENSE`.