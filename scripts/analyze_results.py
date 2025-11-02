#!/usr/bin/env python3
"""
Script de análisis de resultados para simulaciones MANET
Taller de Métodos Estocásticos

Este script analiza los resultados de las simulaciones NS-3 y genera:
- Estadísticas descriptivas
- Gráficos de rendimiento
- Análisis estocástico
- Comparaciones entre protocolos
"""

import sys
import argparse
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy import stats
from pathlib import Path

class MANETAnalyzer:
    """Analizador de resultados de simulaciones MANET"""
    
    def __init__(self, data_file):
        """
        Inicializar analizador
        
        Args:
            data_file: Ruta al archivo CSV con resultados
        """
        self.data_file = data_file
        self.data = None
        self.load_data()
    
    def load_data(self):
        """Cargar datos desde archivo CSV"""
        try:
            self.data = pd.read_csv(self.data_file)
            print(f"✓ Datos cargados: {len(self.data)} filas")
            print(f"  Columnas: {', '.join(self.data.columns)}")
        except FileNotFoundError:
            print(f"✗ Error: No se encontró el archivo {self.data_file}")
            sys.exit(1)
        except Exception as e:
            print(f"✗ Error al cargar datos: {e}")
            sys.exit(1)
    
    def descriptive_statistics(self):
        """Calcular y mostrar estadísticas descriptivas"""
        print("\n" + "="*60)
        print("ESTADÍSTICAS DESCRIPTIVAS")
        print("="*60)
        
        # Métricas principales
        metrics = ['pdr', 'delay', 'throughput', 'overhead']
        
        for metric in metrics:
            if metric in self.data.columns:
                values = self.data[metric].dropna()
                
                print(f"\n{metric.upper()}:")
                print(f"  Media:       {np.mean(values):.4f}")
                print(f"  Mediana:     {np.median(values):.4f}")
                print(f"  Desv. Std:   {np.std(values):.4f}")
                print(f"  Varianza:    {np.var(values):.4f}")
                print(f"  Mínimo:      {np.min(values):.4f}")
                print(f"  Máximo:      {np.max(values):.4f}")
                print(f"  Q1 (25%):    {np.percentile(values, 25):.4f}")
                print(f"  Q2 (50%):    {np.percentile(values, 50):.4f}")
                print(f"  Q3 (75%):    {np.percentile(values, 75):.4f}")
                print(f"  P95:         {np.percentile(values, 95):.4f}")
                print(f"  P99:         {np.percentile(values, 99):.4f}")
    
    def confidence_intervals(self, confidence=0.95):
        """
        Calcular intervalos de confianza
        
        Args:
            confidence: Nivel de confianza (default 0.95)
        """
        print(f"\n" + "="*60)
        print(f"INTERVALOS DE CONFIANZA ({confidence*100}%)")
        print("="*60)
        
        metrics = ['pdr', 'delay', 'throughput']
        
        for metric in metrics:
            if metric in self.data.columns:
                values = self.data[metric].dropna()
                
                mean = np.mean(values)
                std_error = stats.sem(values)
                ci = stats.t.interval(confidence, len(values)-1, 
                                     loc=mean, scale=std_error)
                
                print(f"\n{metric.upper()}:")
                print(f"  Media: {mean:.4f}")
                print(f"  IC {confidence*100}%: [{ci[0]:.4f}, {ci[1]:.4f}]")
                print(f"  Ancho del IC: {ci[1] - ci[0]:.4f}")
    
    def hypothesis_testing(self):
        """Realizar pruebas de hipótesis entre protocolos"""
        if 'protocol' not in self.data.columns:
            print("\n⚠ No se encontró columna 'protocol' para comparaciones")
            return
        
        print("\n" + "="*60)
        print("PRUEBAS DE HIPÓTESIS")
        print("="*60)
        
        protocols = self.data['protocol'].unique()
        
        if len(protocols) < 2:
            print("\n⚠ Se necesitan al menos 2 protocolos para comparar")
            return
        
        # Comparar PDR entre protocolos
        if 'pdr' in self.data.columns:
            print(f"\nComparación de PDR entre protocolos:")
            
            for i in range(len(protocols)):
                for j in range(i+1, len(protocols)):
                    prot1 = protocols[i]
                    prot2 = protocols[j]
                    
                    data1 = self.data[self.data['protocol'] == prot1]['pdr'].dropna()
                    data2 = self.data[self.data['protocol'] == prot2]['pdr'].dropna()
                    
                    # Test t de Student
                    t_stat, p_value = stats.ttest_ind(data1, data2)
                    
                    print(f"\n  {prot1} vs {prot2}:")
                    print(f"    Media {prot1}: {np.mean(data1):.4f}")
                    print(f"    Media {prot2}: {np.mean(data2):.4f}")
                    print(f"    t-statistic: {t_stat:.4f}")
                    print(f"    p-value: {p_value:.4f}")
                    
                    if p_value < 0.05:
                        print(f"    → Diferencia SIGNIFICATIVA (p < 0.05)")
                    else:
                        print(f"    → Diferencia NO significativa (p ≥ 0.05)")
    
    def correlation_analysis(self):
        """Analizar correlaciones entre variables"""
        print("\n" + "="*60)
        print("ANÁLISIS DE CORRELACIÓN")
        print("="*60)
        
        # Seleccionar columnas numéricas
        numeric_cols = self.data.select_dtypes(include=[np.number]).columns
        
        if len(numeric_cols) < 2:
            print("\n⚠ Se necesitan al menos 2 columnas numéricas")
            return
        
        # Calcular matriz de correlación
        corr_matrix = self.data[numeric_cols].corr()
        
        print("\nMatriz de Correlación (Pearson):")
        print(corr_matrix.to_string())
        
        # Identificar correlaciones fuertes
        print("\nCorrelaciones Significativas (|r| > 0.5):")
        for i in range(len(corr_matrix.columns)):
            for j in range(i+1, len(corr_matrix.columns)):
                corr_val = corr_matrix.iloc[i, j]
                if abs(corr_val) > 0.5:
                    col1 = corr_matrix.columns[i]
                    col2 = corr_matrix.columns[j]
                    print(f"  {col1} <-> {col2}: {corr_val:.4f}")
    
    def distribution_fitting(self):
        """Ajustar distribuciones de probabilidad a los datos"""
        print("\n" + "="*60)
        print("AJUSTE DE DISTRIBUCIONES")
        print("="*60)
        
        # Analizar delay (típicamente log-normal)
        if 'delay' in self.data.columns:
            delays = self.data['delay'].dropna()
            delays = delays[delays > 0]  # Eliminar valores no positivos
            
            print("\nDELAY - Ajuste Log-Normal:")
            
            # Ajustar distribución log-normal
            shape, loc, scale = stats.lognorm.fit(delays, floc=0)
            
            print(f"  Parámetros:")
            print(f"    σ (shape): {shape:.4f}")
            print(f"    loc: {loc:.4f}")
            print(f"    scale: {scale:.4f}")
            
            # Test de bondad de ajuste (Kolmogorov-Smirnov)
            ks_stat, ks_pvalue = stats.kstest(delays, 
                                              lambda x: stats.lognorm.cdf(x, shape, loc, scale))
            
            print(f"  Test Kolmogorov-Smirnov:")
            print(f"    Estadístico: {ks_stat:.4f}")
            print(f"    p-value: {ks_pvalue:.4f}")
            
            if ks_pvalue > 0.05:
                print(f"    → Los datos SIGUEN la distribución log-normal")
            else:
                print(f"    → Los datos NO siguen la distribución log-normal")
    
    def plot_results(self, output_dir='results'):
        """
        Generar gráficos de resultados
        
        Args:
            output_dir: Directorio para guardar gráficos
        """
        print("\n" + "="*60)
        print("GENERANDO GRÁFICOS")
        print("="*60)
        
        output_path = Path(output_dir)
        output_path.mkdir(parents=True, exist_ok=True)
        
        # 1. Histogramas de métricas principales
        metrics = ['pdr', 'delay', 'throughput']
        
        for metric in metrics:
            if metric in self.data.columns:
                plt.figure(figsize=(10, 6))
                
                values = self.data[metric].dropna()
                plt.hist(values, bins=30, edgecolor='black', alpha=0.7)
                plt.xlabel(metric.upper())
                plt.ylabel('Frecuencia')
                plt.title(f'Distribución de {metric.upper()}')
                plt.grid(True, alpha=0.3)
                
                filename = output_path / f'hist_{metric}.png'
                plt.savefig(filename, dpi=150, bbox_inches='tight')
                plt.close()
                print(f"  ✓ Generado: {filename}")
        
        # 2. PDR vs Movilidad (si hay datos de velocidad)
        if 'pdr' in self.data.columns and 'speed' in self.data.columns:
            plt.figure(figsize=(10, 6))
            
            if 'protocol' in self.data.columns:
                for protocol in self.data['protocol'].unique():
                    subset = self.data[self.data['protocol'] == protocol]
                    plt.scatter(subset['speed'], subset['pdr'], 
                              label=protocol, alpha=0.6, s=50)
            else:
                plt.scatter(self.data['speed'], self.data['pdr'], 
                          alpha=0.6, s=50)
            
            plt.xlabel('Velocidad de Nodos (m/s)')
            plt.ylabel('Packet Delivery Ratio (%)')
            plt.title('PDR vs Movilidad')
            plt.legend()
            plt.grid(True, alpha=0.3)
            
            filename = output_path / 'pdr_vs_mobility.png'
            plt.savefig(filename, dpi=150, bbox_inches='tight')
            plt.close()
            print(f"  ✓ Generado: {filename}")
        
        # 3. Boxplots de comparación entre protocolos
        if 'protocol' in self.data.columns and 'pdr' in self.data.columns:
            plt.figure(figsize=(10, 6))
            
            protocols = self.data['protocol'].unique()
            data_to_plot = [self.data[self.data['protocol'] == p]['pdr'].dropna() 
                           for p in protocols]
            
            plt.boxplot(data_to_plot, labels=protocols)
            plt.ylabel('Packet Delivery Ratio (%)')
            plt.title('Comparación de PDR entre Protocolos')
            plt.grid(True, alpha=0.3)
            
            filename = output_path / 'boxplot_protocols.png'
            plt.savefig(filename, dpi=150, bbox_inches='tight')
            plt.close()
            print(f"  ✓ Generado: {filename}")
        
        # 4. Matriz de correlación
        numeric_cols = self.data.select_dtypes(include=[np.number]).columns
        if len(numeric_cols) >= 2:
            plt.figure(figsize=(10, 8))
            
            corr_matrix = self.data[numeric_cols].corr()
            
            im = plt.imshow(corr_matrix, cmap='coolwarm', aspect='auto', 
                           vmin=-1, vmax=1)
            plt.colorbar(im)
            
            plt.xticks(range(len(corr_matrix.columns)), corr_matrix.columns, 
                      rotation=45, ha='right')
            plt.yticks(range(len(corr_matrix.columns)), corr_matrix.columns)
            
            # Agregar valores en las celdas
            for i in range(len(corr_matrix.columns)):
                for j in range(len(corr_matrix.columns)):
                    text = plt.text(j, i, f'{corr_matrix.iloc[i, j]:.2f}',
                                   ha="center", va="center", color="black", 
                                   fontsize=8)
            
            plt.title('Matriz de Correlación')
            plt.tight_layout()
            
            filename = output_path / 'correlation_matrix.png'
            plt.savefig(filename, dpi=150, bbox_inches='tight')
            plt.close()
            print(f"  ✓ Generado: {filename}")
        
        print(f"\n✓ Gráficos guardados en: {output_path.absolute()}")


def main():
    """Función principal"""
    parser = argparse.ArgumentParser(
        description='Analizar resultados de simulaciones MANET')
    
    parser.add_argument('data_file', 
                       help='Archivo CSV con resultados de simulación')
    parser.add_argument('-o', '--output', 
                       default='results',
                       help='Directorio de salida para gráficos (default: results)')
    parser.add_argument('--no-plots', 
                       action='store_true',
                       help='No generar gráficos')
    
    args = parser.parse_args()
    
    # Crear analizador
    analyzer = MANETAnalyzer(args.data_file)
    
    # Ejecutar análisis
    analyzer.descriptive_statistics()
    analyzer.confidence_intervals()
    analyzer.hypothesis_testing()
    analyzer.correlation_analysis()
    analyzer.distribution_fitting()
    
    # Generar gráficos
    if not args.no_plots:
        analyzer.plot_results(args.output)
    
    print("\n" + "="*60)
    print("ANÁLISIS COMPLETADO")
    print("="*60)


if __name__ == '__main__':
    main()
