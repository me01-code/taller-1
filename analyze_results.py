#!/usr/bin/env python3
"""
NS-3 MANET Simulation Results Analyzer
Analyzes flow monitor XML output and generates visualization
"""

import xml.etree.ElementTree as ET
import matplotlib.pyplot as plt
import argparse
import sys
import os

def parse_flow_monitor(xml_file):
    """Parse NS-3 FlowMonitor XML output"""
    if not os.path.exists(xml_file):
        print(f"Error: File {xml_file} not found")
        return None
    
    tree = ET.parse(xml_file)
    root = tree.getroot()
    
    flows = []
    
    for flow in root.findall('.//FlowStats/Flow'):
        flow_id = flow.get('flowId')
        tx_bytes = int(flow.get('txBytes', 0))
        rx_bytes = int(flow.get('rxBytes', 0))
        tx_packets = int(flow.get('txPackets', 0))
        rx_packets = int(flow.get('rxPackets', 0))
        lost_packets = int(flow.get('lostPackets', 0))
        delay_sum = float(flow.get('delaySum', 0))
        jitter_sum = float(flow.get('jitterSum', 0))
        
        # Calculate metrics
        delivery_ratio = (rx_packets / tx_packets * 100) if tx_packets > 0 else 0
        avg_delay = (delay_sum / rx_packets / 1e9) if rx_packets > 0 else 0  # Convert to seconds
        avg_jitter = (jitter_sum / (rx_packets - 1) / 1e9) if rx_packets > 1 else 0  # Convert to seconds
        
        flows.append({
            'id': flow_id,
            'tx_bytes': tx_bytes,
            'rx_bytes': rx_bytes,
            'tx_packets': tx_packets,
            'rx_packets': rx_packets,
            'lost_packets': lost_packets,
            'delivery_ratio': delivery_ratio,
            'avg_delay': avg_delay,
            'avg_jitter': avg_jitter
        })
    
    return flows

def print_statistics(flows):
    """Print flow statistics"""
    print("\n" + "="*80)
    print("FLOW STATISTICS")
    print("="*80)
    
    total_tx_packets = 0
    total_rx_packets = 0
    total_lost_packets = 0
    
    for flow in flows:
        print(f"\nFlow {flow['id']}:")
        print(f"  TX Packets: {flow['tx_packets']}")
        print(f"  RX Packets: {flow['rx_packets']}")
        print(f"  Lost Packets: {flow['lost_packets']}")
        print(f"  Delivery Ratio: {flow['delivery_ratio']:.2f}%")
        print(f"  Average Delay: {flow['avg_delay']*1000:.2f} ms")
        print(f"  Average Jitter: {flow['avg_jitter']*1000:.2f} ms")
        
        total_tx_packets += flow['tx_packets']
        total_rx_packets += flow['rx_packets']
        total_lost_packets += flow['lost_packets']
    
    print("\n" + "="*80)
    print("OVERALL STATISTICS")
    print("="*80)
    print(f"Total TX Packets: {total_tx_packets}")
    print(f"Total RX Packets: {total_rx_packets}")
    print(f"Total Lost Packets: {total_lost_packets}")
    print(f"Overall Delivery Ratio: {(total_rx_packets/total_tx_packets*100) if total_tx_packets > 0 else 0:.2f}%")
    print("="*80 + "\n")

def plot_results(flows, output_file='flow_analysis.png'):
    """Generate visualization plots"""
    if not flows:
        print("No flows to plot")
        return
    
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))
    fig.suptitle('NS-3 MANET Simulation Analysis', fontsize=16)
    
    flow_ids = [f['id'] for f in flows]
    
    # Plot 1: Packet Delivery Ratio
    delivery_ratios = [f['delivery_ratio'] for f in flows]
    axes[0, 0].bar(flow_ids, delivery_ratios, color='skyblue')
    axes[0, 0].set_xlabel('Flow ID')
    axes[0, 0].set_ylabel('Delivery Ratio (%)')
    axes[0, 0].set_title('Packet Delivery Ratio per Flow')
    axes[0, 0].set_ylim([0, 105])
    axes[0, 0].grid(axis='y', alpha=0.3)
    
    # Plot 2: Average Delay
    delays = [f['avg_delay']*1000 for f in flows]  # Convert to ms
    axes[0, 1].bar(flow_ids, delays, color='lightcoral')
    axes[0, 1].set_xlabel('Flow ID')
    axes[0, 1].set_ylabel('Average Delay (ms)')
    axes[0, 1].set_title('Average Delay per Flow')
    axes[0, 1].grid(axis='y', alpha=0.3)
    
    # Plot 3: Packets Comparison
    tx_packets = [f['tx_packets'] for f in flows]
    rx_packets = [f['rx_packets'] for f in flows]
    x = range(len(flow_ids))
    width = 0.35
    axes[1, 0].bar([i - width/2 for i in x], tx_packets, width, label='TX Packets', color='green', alpha=0.7)
    axes[1, 0].bar([i + width/2 for i in x], rx_packets, width, label='RX Packets', color='blue', alpha=0.7)
    axes[1, 0].set_xlabel('Flow ID')
    axes[1, 0].set_ylabel('Packet Count')
    axes[1, 0].set_title('TX vs RX Packets per Flow')
    axes[1, 0].set_xticks(x)
    axes[1, 0].set_xticklabels(flow_ids)
    axes[1, 0].legend()
    axes[1, 0].grid(axis='y', alpha=0.3)
    
    # Plot 4: Average Jitter
    jitters = [f['avg_jitter']*1000 for f in flows]  # Convert to ms
    axes[1, 1].bar(flow_ids, jitters, color='lightgreen')
    axes[1, 1].set_xlabel('Flow ID')
    axes[1, 1].set_ylabel('Average Jitter (ms)')
    axes[1, 1].set_title('Average Jitter per Flow')
    axes[1, 1].grid(axis='y', alpha=0.3)
    
    plt.tight_layout()
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"\nPlot saved to {output_file}")
    
    # Try to display the plot
    try:
        plt.show()
    except:
        print("(Display not available, plot saved to file)")

def main():
    parser = argparse.ArgumentParser(description='Analyze NS-3 MANET simulation results')
    parser.add_argument('xml_file', type=str, help='Path to FlowMonitor XML file')
    parser.add_argument('--output', type=str, default='flow_analysis.png',
                        help='Output plot filename (default: flow_analysis.png)')
    parser.add_argument('--no-plot', action='store_true',
                        help='Skip plotting, only print statistics')
    
    args = parser.parse_args()
    
    # Parse flow monitor data
    flows = parse_flow_monitor(args.xml_file)
    
    if not flows:
        print("Failed to parse flow data")
        return 1
    
    # Print statistics
    print_statistics(flows)
    
    # Generate plots
    if not args.no_plot:
        try:
            plot_results(flows, args.output)
        except Exception as e:
            print(f"Error generating plots: {e}")
            print("Statistics printed successfully, but plotting failed")
    
    return 0

if __name__ == "__main__":
    sys.exit(main())
