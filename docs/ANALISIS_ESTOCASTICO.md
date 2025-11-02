# Análisis Estocástico de Redes MANET

Este documento describe los métodos estocásticos aplicados al análisis de protocolos de enrutamiento en redes MANET.

## Índice

1. [Fundamentos Estocásticos](#fundamentos-estocásticos)
2. [Modelado de Movilidad](#modelado-de-movilidad)
3. [Análisis de Tráfico](#análisis-de-tráfico)
4. [Distribuciones de Probabilidad](#distribuciones-de-probabilidad)
5. [Procesos Estocásticos](#procesos-estocásticos)
6. [Análisis de Rendimiento](#análisis-de-rendimiento)

## Fundamentos Estocásticos

### Variables Aleatorias en MANET

En redes MANET, múltiples aspectos son inherentemente aleatorios:

1. **Posición de nodos**: Movimiento aleatorio de dispositivos móviles
2. **Llegada de paquetes**: Tiempos de llegada siguen procesos estocásticos
3. **Calidad de canal**: Variabilidad en la propagación de señales
4. **Eventos de fallo**: Pérdida de paquetes, desconexiones

### Notación Matemática

- **X(t)**: Variable aleatoria en el tiempo t
- **E[X]**: Valor esperado de X
- **Var(X)**: Varianza de X
- **P(X)**: Función de probabilidad de X
- **f_X(x)**: Función de densidad de probabilidad

## Modelado de Movilidad

### Random Waypoint Model

Modelo más común en simulaciones MANET.

#### Características

```
1. Seleccionar destino aleatorio (x, y) uniformemente en el área
2. Seleccionar velocidad v ~ U[v_min, v_max]
3. Moverse hacia el destino a velocidad v
4. Al llegar, pausar por tiempo t_pause
5. Repetir
```

#### Distribución de Posiciones

Para un área A de dimensiones L × W:

```
f_X(x) = 1/L,  para 0 ≤ x ≤ L
f_Y(y) = 1/W,  para 0 ≤ y ≤ W
```

#### Velocidad

```
V ~ U[v_min, v_max]
E[V] = (v_min + v_max) / 2
Var(V) = (v_max - v_min)² / 12
```

### Random Walk Model

#### Descripción

Movimiento basado en pasos aleatorios discretos:

```
X(t+1) = X(t) + ΔX
Y(t+1) = Y(t) + ΔY

donde ΔX, ΔY ~ N(0, σ²)
```

#### Propiedades Estadísticas

```
E[X(t)] = X(0)
Var(X(t)) = tσ²
```

### Gauss-Markov Mobility Model

Modelo más realista que incorpora correlación temporal:

```
v(t+1) = αv(t) + (1-α)v̄ + √(1-α²)·w_v(t)
θ(t+1) = αθ(t) + (1-α)θ̄ + √(1-α²)·w_θ(t)
```

donde:
- α: parámetro de memoria (0 ≤ α ≤ 1)
- v̄, θ̄: valores medios
- w_v, w_θ: ruido gaussiano

## Análisis de Tráfico

### Modelo de Llegada de Paquetes

#### Proceso de Poisson

Para tráfico CBR (Constant Bit Rate):

```
P(N(t) = n) = (λt)^n · e^(-λt) / n!
```

donde:
- λ: tasa de llegada (paquetes/segundo)
- N(t): número de paquetes en el intervalo [0,t]

#### Propiedades

```
E[N(t)] = λt
Var(N(t)) = λt
```

#### Tiempo entre Llegadas

```
T ~ Exp(λ)
f_T(t) = λe^(-λt),  t ≥ 0
E[T] = 1/λ
```

### Distribución del Tamaño de Paquetes

Típicamente modelado como:

```
S ~ N(μ, σ²)  o  S = constante (para CBR)
```

## Distribuciones de Probabilidad

### Delay (Retardo End-to-End)

Típicamente sigue una distribución **log-normal** o **gamma**:

#### Distribución Log-Normal

```
f_D(d) = (1 / (d·σ·√(2π))) · exp(-(ln(d) - μ)² / (2σ²))
```

#### Parámetros en NS-3

```python
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import lognorm

# Datos de simulación
delays = [...]  # delays en milisegundos

# Ajustar distribución log-normal
shape, loc, scale = lognorm.fit(delays)

# Graficar
x = np.linspace(0, max(delays), 100)
plt.hist(delays, bins=30, density=True, alpha=0.6, label='Datos')
plt.plot(x, lognorm.pdf(x, shape, loc, scale), 'r-', label='Log-Normal')
plt.xlabel('Delay (ms)')
plt.ylabel('Densidad de Probabilidad')
plt.legend()
```

### Packet Loss

Modelado con distribución **Binomial** o **Bernoulli**:

```
X ~ Binomial(n, p)
P(X = k) = C(n,k) · p^k · (1-p)^(n-k)
```

donde:
- n: número de paquetes enviados
- p: probabilidad de pérdida
- k: número de paquetes perdidos

#### PDR (Packet Delivery Ratio)

```
PDR = (n - X) / n
E[PDR] = 1 - p
Var(PDR) = p(1-p) / n
```

### Throughput

Puede modelarse como **distribución normal** en condiciones estables:

```
Θ ~ N(μ_θ, σ_θ²)
```

## Procesos Estocásticos

### Cadenas de Markov

#### Estados de Conectividad

Modelar conectividad entre nodos como cadena de Markov:

```
Estados: {Conectado, Desconectado}

Matriz de transición P:
P = | p_cc  p_cd |
    | p_dc  p_dd |

donde:
p_cc: probabilidad de permanecer conectado
p_cd: probabilidad de desconectarse
p_dc: probabilidad de reconectarse
p_dd: probabilidad de permanecer desconectado
```

#### Distribución Estacionaria

```
π = πP
π_c = p_dc / (p_cd + p_dc)
π_d = p_cd / (p_cd + p_dc)
```

### Procesos de Renovación

Para modelar eventos de reconfiguración de red:

```
N(t): número de cambios de topología en [0,t]
T_i: tiempo entre cambios

Si T_i ~ Exp(λ), entonces N(t) ~ Poisson(λt)
```

### Procesos de Nacimiento y Muerte

Para modelar entrada/salida de nodos:

```
λ_n: tasa de llegada de nuevos nodos
μ_n: tasa de salida de nodos

Estado estable: λ_n = μ_n
```

## Análisis de Rendimiento

### Métricas Estadísticas

#### 1. Estadísticas Descriptivas

```python
import numpy as np
import pandas as pd

# Cargar datos
data = pd.read_csv('results.csv')

# Estadísticas básicas
print(f"Media: {np.mean(data['pdr'])}")
print(f"Mediana: {np.median(data['pdr'])}")
print(f"Desviación estándar: {np.std(data['pdr'])}")
print(f"Varianza: {np.var(data['pdr'])}")
print(f"Min: {np.min(data['pdr'])}, Max: {np.max(data['pdr'])}")
```

#### 2. Percentiles

```python
# Calcular percentiles
p25 = np.percentile(data['delay'], 25)
p50 = np.percentile(data['delay'], 50)  # mediana
p75 = np.percentile(data['delay'], 75)
p95 = np.percentile(data['delay'], 95)
p99 = np.percentile(data['delay'], 99)

print(f"P25: {p25}, P50: {p50}, P75: {p75}")
print(f"P95: {p95}, P99: {p99}")
```

### Intervalos de Confianza

Para estimar el valor medio con confianza del 95%:

```python
from scipy import stats

# Intervalo de confianza para la media
mean = np.mean(data)
std_error = stats.sem(data)
confidence_interval = stats.t.interval(
    0.95,
    len(data) - 1,
    loc=mean,
    scale=std_error
)

print(f"IC 95%: [{confidence_interval[0]:.2f}, {confidence_interval[1]:.2f}]")
```

### Pruebas de Hipótesis

#### Test t de Student

Comparar dos protocolos:

```python
from scipy.stats import ttest_ind

# Datos de dos protocolos
olsr_pdr = [...]
batman_pdr = [...]

# Hipótesis:
# H0: μ_olsr = μ_batman
# H1: μ_olsr ≠ μ_batman

t_stat, p_value = ttest_ind(olsr_pdr, batman_pdr)

alpha = 0.05
if p_value < alpha:
    print(f"Rechazar H0 (p={p_value:.4f})")
    print("Hay diferencia significativa entre protocolos")
else:
    print(f"No rechazar H0 (p={p_value:.4f})")
    print("No hay diferencia significativa")
```

#### ANOVA (Analysis of Variance)

Para comparar múltiples escenarios:

```python
from scipy.stats import f_oneway

# Datos de diferentes escenarios
scenario1 = [...]
scenario2 = [...]
scenario3 = [...]

# ANOVA de una vía
f_stat, p_value = f_oneway(scenario1, scenario2, scenario3)

print(f"F-statistic: {f_stat:.4f}")
print(f"p-value: {p_value:.4f}")
```

### Análisis de Correlación

#### Correlación entre Métricas

```python
# Calcular matriz de correlación
correlation_matrix = data[['pdr', 'delay', 'throughput']].corr()

print(correlation_matrix)

# Visualizar
import seaborn as sns
sns.heatmap(correlation_matrix, annot=True, cmap='coolwarm')
plt.title('Matriz de Correlación')
plt.show()
```

#### Correlación vs Movilidad

```python
from scipy.stats import pearsonr

# Correlación entre velocidad y PDR
corr, p_value = pearsonr(data['speed'], data['pdr'])

print(f"Correlación de Pearson: {corr:.4f}")
print(f"p-value: {p_value:.4f}")
```

### Regresión

#### Regresión Lineal

```python
from sklearn.linear_model import LinearRegression
import numpy as np

# Predecir PDR basado en velocidad
X = data['speed'].values.reshape(-1, 1)
y = data['pdr'].values

model = LinearRegression()
model.fit(X, y)

print(f"Coeficiente: {model.coef_[0]:.4f}")
print(f"Intercepción: {model.intercept_:.4f}")
print(f"R²: {model.score(X, y):.4f}")

# Predicción
speed_test = np.array([[15]])
pdr_pred = model.predict(speed_test)
print(f"PDR predicho para velocidad {speed_test[0][0]}: {pdr_pred[0]:.2f}")
```

### Simulación Monte Carlo

Para estimar comportamiento bajo incertidumbre:

```python
import numpy as np

def simulate_network(n_nodes, speed, duration, n_simulations=1000):
    """Simulación Monte Carlo de red MANET"""
    pdrs = []
    
    for _ in range(n_simulations):
        # Simular con parámetros aleatorios
        noise = np.random.normal(0, 0.1)
        pdr = base_pdr(n_nodes, speed) + noise
        pdr = np.clip(pdr, 0, 100)  # Limitar entre 0 y 100%
        pdrs.append(pdr)
    
    return np.array(pdrs)

# Ejecutar simulación
results = simulate_network(n_nodes=30, speed=10, duration=100)

# Analizar resultados
print(f"PDR medio: {np.mean(results):.2f}%")
print(f"Desviación estándar: {np.std(results):.2f}%")
print(f"IC 95%: [{np.percentile(results, 2.5):.2f}, "
      f"{np.percentile(results, 97.5):.2f}]")
```

## Aplicaciones Prácticas

### 1. Predicción de Rendimiento

Usar modelos estocásticos para predecir comportamiento de red bajo diferentes condiciones.

### 2. Dimensionamiento de Red

Determinar número óptimo de nodos para cumplir requisitos de QoS.

### 3. Optimización de Parámetros

Encontrar configuraciones óptimas de protocolos usando análisis estocástico.

### 4. Análisis de Confiabilidad

Calcular probabilidades de fallo y tiempo medio entre fallos (MTBF).

## Ejercicios Prácticos

### Ejercicio 1: Distribución de Delays

1. Ejecutar simulación y recolectar datos de delay
2. Ajustar distribución log-normal
3. Calcular intervalos de confianza
4. Comparar entre protocolos

### Ejercicio 2: Proceso de Poisson

1. Analizar llegada de paquetes
2. Verificar si sigue proceso de Poisson
3. Estimar tasa λ
4. Validar con test chi-cuadrado

### Ejercicio 3: Cadena de Markov

1. Modelar estados de conectividad
2. Estimar matriz de transición
3. Calcular distribución estacionaria
4. Predecir comportamiento a largo plazo

## Referencias Bibliográficas

1. Ross, S. M. (2014). *Introduction to Probability Models*. Academic Press.
2. Bolch, G. et al. (2006). *Queueing Networks and Markov Chains*. Wiley.
3. Camp, T. et al. (2002). "A survey of mobility models for ad hoc network research". *Wireless Communications and Mobile Computing*.
4. Bettstetter, C. (2001). "Smooth is better than sharp: a random mobility model for simulation of wireless networks". *ACM MSWiM*.

## Herramientas Recomendadas

- **Python**: NumPy, SciPy, Pandas, Matplotlib
- **R**: Para análisis estadístico avanzado
- **MATLAB**: Para simulaciones y análisis
- **Jupyter Notebook**: Para documentar análisis

## Conclusión

El análisis estocástico es fundamental para:
- Entender comportamiento de redes MANET
- Predecir rendimiento bajo incertidumbre
- Optimizar configuraciones de protocolos
- Validar resultados de simulación

El uso de métodos estocásticos permite tomar decisiones informadas sobre diseño y despliegue de redes móviles ad-hoc.
