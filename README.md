# NS-3 MANET Simulation Project

This repository contains NS-3 simulations for Mobile Ad-hoc Networks (MANETs) using different routing protocols including AODV and OLSR.

## Overview

Mobile Ad-hoc Networks (MANETs) are self-configuring networks of mobile devices connected wirelessly. This project provides NS-3 simulation scripts to evaluate and compare different MANET routing protocols.

### Included Routing Protocols

- **AODV (Ad-hoc On-Demand Distance Vector)**: Reactive routing protocol that discovers routes only when needed
- **OLSR (Optimized Link State Routing)**: Proactive routing protocol that maintains routes to all destinations

## Project Structure

```
.
├── manet-aodv.cc              # AODV routing protocol simulation
├── manet-olsr.cc              # OLSR routing protocol simulation
├── run_manet_simulation.py    # Python script to run simulations
├── analyze_results.py         # Python script to analyze results
├── scenarios/                 # Example scenario configurations
└── README.md                  # This file
```

## Prerequisites

### NS-3 Installation

1. **Download NS-3**:
   ```bash
   wget https://www.nsnam.org/releases/ns-allinone-3.38.tar.bz2
   tar xjf ns-allinone-3.38.tar.bz2
   cd ns-allinone-3.38
   ```

2. **Build NS-3**:
   ```bash
   ./build.py --enable-examples --enable-tests
   ```

3. **Verify installation**:
   ```bash
   cd ns-3.38
   ./waf --run hello-simulator
   ```

### Required Python Packages

```bash
pip install matplotlib numpy
```

## Usage

### Method 1: Using Python Runner Script

The easiest way to run simulations:

```bash
# Run both AODV and OLSR simulations
python3 run_manet_simulation.py --protocol both --nWifis 10 --totalTime 100

# Run only AODV simulation
python3 run_manet_simulation.py --protocol aodv --nWifis 15 --nodeSpeed 20

# Run only OLSR simulation
python3 run_manet_simulation.py --protocol olsr --nSinks 5
```

### Method 2: Manual NS-3 Compilation

1. **Copy simulation files to NS-3 scratch directory**:
   ```bash
   cp manet-aodv.cc /path/to/ns-3.38/scratch/
   cp manet-olsr.cc /path/to/ns-3.38/scratch/
   ```

2. **Build the simulations**:
   ```bash
   cd /path/to/ns-3.38
   ./waf build
   ```

3. **Run simulations**:
   ```bash
   # AODV simulation
   ./waf --run "manet-aodv --nWifis=10 --totalTime=100"
   
   # OLSR simulation
   ./waf --run "manet-olsr --nWifis=10 --totalTime=100"
   ```

## Simulation Parameters

Both simulations accept the following command-line parameters:

| Parameter | Description | Default Value |
|-----------|-------------|---------------|
| `--nWifis` | Number of WiFi nodes | 10 |
| `--nSinks` | Number of sink (receiver) nodes | 3 |
| `--totalTime` | Total simulation time (seconds) | 100.0 |
| `--rate` | CBR traffic rate | 2048bps |
| `--phyMode` | WiFi PHY mode | DsssRate11Mbps |
| `--nodeSpeed` | Node speed in m/s | 10 |
| `--nodePause` | Pause time in seconds | 0 |

### Example Commands

```bash
# High mobility scenario
./waf --run "manet-aodv --nWifis=20 --nodeSpeed=20 --totalTime=200"

# Low mobility scenario
./waf --run "manet-olsr --nWifis=15 --nodeSpeed=5 --nodePause=10"

# Dense network
./waf --run "manet-aodv --nWifis=30 --nSinks=10"
```

## Analyzing Results

### Flow Monitor Analysis

The simulations automatically generate flow statistics. To analyze the results in detail:

```bash
# Analyze FlowMonitor XML output
python3 analyze_results.py flowmon.xml
```

This will:
- Print detailed statistics for each flow
- Calculate overall packet delivery ratio
- Generate visualization plots showing:
  - Packet delivery ratio per flow
  - Average delay per flow
  - TX vs RX packets comparison
  - Average jitter per flow

### Output Files

After running a simulation, you'll find:
- `manet-aodv-*.pcap` or `manet-olsr-*.pcap`: Packet capture files
- Flow monitor statistics in console output
- Optional FlowMonitor XML files for detailed analysis

## Simulation Features

### Network Configuration
- **Topology**: Random waypoint mobility model
- **Area**: 500m x 500m
- **WiFi Standard**: 802.11b (ad-hoc mode)
- **Addressing**: IPv4 (10.1.1.0/24)
- **Transport**: UDP

### Traffic Model
- **Application**: On-Off Application (CBR traffic)
- **Pattern**: Constant bit rate
- **Packet Size**: 1024 bytes
- **Receivers**: PacketSink applications

### Mobility Model
- **Type**: Random Waypoint Mobility Model
- **Speed**: Configurable (default 10 m/s)
- **Pause**: Configurable (default 0 seconds)
- **Area**: 500m x 500m rectangle

## Performance Metrics

The simulations measure:
- **Throughput**: Data rate achieved per flow
- **Packet Delivery Ratio**: Percentage of packets successfully delivered
- **End-to-End Delay**: Average time for packets to reach destination
- **Jitter**: Variation in packet delay
- **Packet Loss**: Number and percentage of lost packets

## Comparing Protocols

### AODV vs OLSR

**AODV (Reactive)**:
- ✅ Lower overhead in low traffic scenarios
- ✅ Better scalability
- ❌ Higher initial route discovery delay
- ❌ Route breaks need rediscovery

**OLSR (Proactive)**:
- ✅ Routes always available
- ✅ Lower latency for first packet
- ❌ Higher control overhead
- ❌ Less efficient in large sparse networks

## Troubleshooting

### Common Issues

1. **NS-3 not found**:
   - Ensure NS-3 is properly installed
   - Provide NS-3 path: `python3 run_manet_simulation.py --ns3-path /path/to/ns-3.38`

2. **Build failures**:
   - Check that all NS-3 modules are enabled
   - Verify compiler compatibility (GCC 7+ or Clang 7+)

3. **No output**:
   - Check simulation time is sufficient
   - Verify node parameters (nWifis > nSinks)

## Advanced Usage

### Custom Scenarios

Create custom scenarios by modifying simulation parameters:

```cpp
// In manet-aodv.cc or manet-olsr.cc
int nWifis = 25;          // Larger network
int nSinks = 5;           // More receivers
double totalTime = 200.0; // Longer simulation
uint32_t nodeSpeed = 15;  // Faster nodes
```

### Adding More Protocols

To add additional routing protocols (e.g., DSR, DSDV):
1. Create new simulation file (e.g., `manet-dsr.cc`)
2. Include appropriate routing module
3. Configure routing helper
4. Follow the same structure as existing examples

## References

- [NS-3 Documentation](https://www.nsnam.org/documentation/)
- [AODV RFC 3561](https://tools.ietf.org/html/rfc3561)
- [OLSR RFC 3626](https://tools.ietf.org/html/rfc3626)
- [NS-3 MANET Routing](https://www.nsnam.org/docs/release/3.38/models/html/routing-overview.html)

## Contributing

Feel free to submit issues, fork the repository, and create pull requests for any improvements.

## License

See LICENSE file for details.
