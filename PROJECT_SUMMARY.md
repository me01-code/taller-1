# NS-3 MANET Project Summary

## Project Overview

This project implements a complete Mobile Ad-hoc Network (MANET) simulation environment using NS-3 (Network Simulator 3). It provides ready-to-use implementations of AODV and OLSR routing protocols with comprehensive documentation, helper scripts, and pre-configured scenarios.

## What is a MANET?

A Mobile Ad-hoc Network (MANET) is a wireless network where nodes can move freely and communicate without fixed infrastructure. Each node acts as both a host and a router, forwarding packets for other nodes. MANETs are used in:

- Emergency and disaster response
- Military tactical communications
- Vehicular networks
- IoT and sensor networks
- Temporary event networks

## Implemented Routing Protocols

### 1. AODV (Ad-hoc On-Demand Distance Vector)

**Type:** Reactive (on-demand) routing protocol

**Characteristics:**
- Discovers routes only when needed
- Maintains only active routes
- Lower overhead in sparse or low-traffic networks
- Route discovery latency on first packet

**Best For:**
- Networks with intermittent communication
- Large networks with sparse traffic
- Battery-constrained devices
- Dynamic topologies

**Implementation:** `manet-aodv.cc`

### 2. OLSR (Optimized Link State Routing)

**Type:** Proactive (table-driven) routing protocol

**Characteristics:**
- Maintains routes to all destinations continuously
- Uses Multi-Point Relays (MPRs) to reduce overhead
- Routes immediately available (no discovery delay)
- Higher control overhead

**Best For:**
- Networks requiring low latency
- Dense networks with frequent communication
- Real-time applications
- Relatively stable topologies

**Implementation:** `manet-olsr.cc`

## Project Structure

```
taller-1/
├── manet-aodv.cc              # AODV protocol simulation (239 lines)
├── manet-olsr.cc              # OLSR protocol simulation (233 lines)
├── run_manet_simulation.py    # Automated simulation runner
├── analyze_results.py         # Results analysis and visualization
├── install.sh                 # One-command installation script
├── Makefile                   # Build automation
├── README.md                  # Complete documentation
├── QUICKSTART.md              # 5-minute getting started guide
├── CONTRIBUTING.md            # Contribution guidelines
├── example-config.txt         # Example configurations
└── scenarios/                 # Pre-configured test scenarios
    ├── low-mobility.sh        # Pedestrian network scenario
    ├── high-mobility.sh       # Vehicular network scenario
    ├── dense-network.sh       # Urban environment scenario
    └── README.md              # Scenarios documentation
```

## Key Features

### Simulation Capabilities

1. **Flexible Network Configuration**
   - Configurable number of nodes (recommended: 5-40)
   - Adjustable sender/receiver ratios
   - Customizable simulation duration

2. **Realistic Mobility Models**
   - Random Waypoint Mobility Model
   - Configurable speed (0-40 m/s)
   - Adjustable pause times
   - 500m × 500m simulation area

3. **Traffic Generation**
   - UDP-based On-Off application
   - Configurable traffic rates
   - Multiple simultaneous flows
   - PacketSink receivers

4. **Performance Metrics**
   - Throughput (bits per second)
   - Packet Delivery Ratio (PDR)
   - End-to-End Delay
   - Jitter
   - Packet loss statistics

5. **Network Configuration**
   - IEEE 802.11b WiFi (ad-hoc mode)
   - IPv4 addressing (10.1.1.0/24)
   - Constant rate transmission (11 Mbps)

### Helper Tools

1. **Python Runner** (`run_manet_simulation.py`)
   - Automatic NS-3 detection
   - Build management
   - Parameter validation
   - Protocol comparison

2. **Results Analyzer** (`analyze_results.py`)
   - FlowMonitor XML parsing
   - Statistical analysis
   - Matplotlib visualizations
   - Multiple plot types

3. **Installation Script** (`install.sh`)
   - NS-3 verification
   - Automatic file copying
   - Build testing
   - Dependency checking

4. **Makefile**
   - Convenient build commands
   - Scenario shortcuts
   - Clean operations
   - Path management

## Usage Examples

### Basic Usage

```bash
# Quick installation
./install.sh /path/to/ns-3.38

# Run AODV simulation
cd /path/to/ns-3.38
./waf --run manet-aodv

# Run with custom parameters
./waf --run "manet-aodv --nWifis=15 --nodeSpeed=20 --totalTime=50"
```

### Using Python Runner

```bash
# Run both protocols
python3 run_manet_simulation.py --protocol both --nWifis 10 --totalTime 100

# Run with custom parameters
python3 run_manet_simulation.py --protocol aodv --nWifis 20 --nodeSpeed 15
```

### Using Makefile

```bash
# Install and build
make install NS3_PATH=/path/to/ns-3.38

# Run simulations
make run-aodv ARGS="--nWifis=15"
make run-all

# Run scenarios
make scenario-low-mobility
```

### Using Pre-configured Scenarios

```bash
# From NS-3 directory
bash /path/to/taller-1/scenarios/low-mobility.sh
bash /path/to/taller-1/scenarios/high-mobility.sh
bash /path/to/taller-1/scenarios/dense-network.sh
```

## Configuration Parameters

| Parameter | Range | Default | Description |
|-----------|-------|---------|-------------|
| `--nWifis` | 2-50 | 10 | Total number of nodes |
| `--nSinks` | 1-nWifis | 3 | Number of receiver nodes |
| `--totalTime` | 10-1000 | 100 | Simulation duration (seconds) |
| `--rate` | any | 2048bps | Traffic generation rate |
| `--phyMode` | WiFi modes | DsssRate11Mbps | Physical layer mode |
| `--nodeSpeed` | 0-50 | 10 | Maximum node speed (m/s) |
| `--nodePause` | 0-60 | 0 | Pause duration (seconds) |

## Performance Metrics Explained

### Throughput
- **Definition:** Amount of data successfully delivered per unit time
- **Unit:** Mbps (Megabits per second)
- **Calculation:** (Received Bytes × 8) / Time Duration / 1,000,000
- **Good Values:** Depends on traffic rate and network conditions

### Packet Delivery Ratio (PDR)
- **Definition:** Percentage of packets successfully received
- **Unit:** Percentage (%)
- **Calculation:** (RX Packets / TX Packets) × 100
- **Good Values:** >90% for stable networks, >70% for high mobility

### End-to-End Delay
- **Definition:** Time taken for a packet to travel from source to destination
- **Unit:** Milliseconds (ms)
- **Good Values:** <50ms for real-time apps, <200ms for interactive apps

### Jitter
- **Definition:** Variation in packet arrival times
- **Unit:** Milliseconds (ms)
- **Good Values:** <30ms for VoIP, <50ms for video streaming

## Expected Results

### Low Mobility Scenario
- **OLSR:** Better PDR (~95-98%), lower delay
- **AODV:** Good PDR (~90-95%), slightly higher delay
- **Reason:** Stable topology favors proactive routing

### High Mobility Scenario
- **AODV:** Better adaptation, PDR (~75-85%)
- **OLSR:** More route inconsistencies, PDR (~70-80%)
- **Reason:** Frequent topology changes favor reactive routing

### Dense Network Scenario
- **AODV:** Better scalability, moderate overhead
- **OLSR:** Higher control overhead, more MPR optimizations
- **Both:** Increased interference, lower throughput per node

## Dependencies

### Required
- **NS-3** (version 3.35 or later)
  - Download: https://www.nsnam.org/releases/
  - Modules: core, network, internet, wifi, aodv, olsr, applications, mobility, flow-monitor
- **C++ Compiler** (GCC 7+ or Clang 7+)
- **Python 3** (3.6 or later)

### Optional
- **matplotlib** (for visualization)
  - Install: `pip3 install matplotlib`
- **numpy** (for numerical analysis)
  - Install: `pip3 install numpy`

## Extending the Project

### Adding New Routing Protocols

1. Create new file (e.g., `manet-dsr.cc`)
2. Include appropriate module (`#include "ns3/dsr-module.h"`)
3. Use the routing helper (e.g., `DsrHelper`)
4. Follow existing simulation structure
5. Update documentation

### Adding Custom Scenarios

1. Create shell script in `scenarios/`
2. Define parameters for your use case
3. Run both protocols for comparison
4. Document expected outcomes

### Enhancing Analysis

1. Extend `analyze_results.py`
2. Add new visualization types
3. Calculate additional metrics
4. Export data in different formats

## Troubleshooting

### Common Issues

**Issue:** NS-3 not found
- **Solution:** Specify path: `--ns3-path /correct/path`

**Issue:** Build failures
- **Solution:** Verify NS-3 modules are enabled, rebuild NS-3

**Issue:** Low packet delivery ratio
- **Solution:** Increase simulation time, adjust node parameters

**Issue:** No flows recorded
- **Solution:** Ensure nWifis > nSinks, check traffic rate

## Educational Value

This project is ideal for:

### Students
- Understanding MANET concepts
- Learning NS-3 simulation
- Comparing routing protocols
- Analyzing network performance

### Researchers
- Protocol evaluation baseline
- Performance comparison framework
- Mobility model testing
- Network optimization studies

### Developers
- NS-3 development examples
- C++ simulation patterns
- Python automation scripts
- Documentation best practices

## Performance Tips

1. **Start Small:** Begin with 5-10 nodes for quick testing
2. **Incremental Testing:** Gradually increase parameters
3. **Multiple Runs:** Run simulations 5-10 times for statistical significance
4. **Parameter Sweeps:** Vary one parameter at a time
5. **Compare Protocols:** Always test both AODV and OLSR with same parameters

## References

### AODV
- RFC 3561: https://tools.ietf.org/html/rfc3561
- Paper: C. Perkins et al., "Ad hoc On-Demand Distance Vector (AODV) Routing"

### OLSR
- RFC 3626: https://tools.ietf.org/html/rfc3626
- Paper: T. Clausen et al., "Optimized Link State Routing Protocol (OLSR)"

### NS-3
- Official Website: https://www.nsnam.org/
- Documentation: https://www.nsnam.org/documentation/
- Tutorial: https://www.nsnam.org/docs/tutorial/html/

### MANET General
- IETF MANET Working Group: https://datatracker.ietf.org/wg/manet/
- Mobile Ad Hoc Networking (MANET): Routing Protocol Performance Issues and Evaluation Considerations

## License

This project follows the license specified in the LICENSE file. The NS-3 simulator itself is licensed under the GNU GPLv2.

## Acknowledgments

- NS-3 development team for the excellent network simulator
- IETF MANET working group for protocol specifications
- Open source community for tools and libraries

## Future Enhancements

Potential improvements for this project:

1. **Additional Protocols:**
   - DSR (Dynamic Source Routing)
   - DSDV (Destination-Sequenced Distance-Vector)
   - BATMAN (Better Approach To Mobile Adhoc Networking)
   - ZRP (Zone Routing Protocol)

2. **Enhanced Analysis:**
   - Real-time visualization with NetAnim
   - Automated parameter optimization
   - Machine learning for protocol selection
   - Energy consumption analysis

3. **Advanced Scenarios:**
   - Multi-interface nodes
   - Heterogeneous networks
   - QoS-aware routing
   - Security considerations

4. **Integration:**
   - Docker containerization
   - CI/CD pipeline
   - Web-based visualization
   - Cloud simulation platform

## Contact and Support

For questions, issues, or contributions:
- GitHub Issues: Report bugs or request features
- Pull Requests: Submit improvements
- Documentation: Refer to README.md and CONTRIBUTING.md

---

**Project Statistics:**
- Lines of C++ code: 472
- Lines of Python code: ~300
- Lines of documentation: ~1000
- Number of files: 15
- Pre-configured scenarios: 3
- Supported routing protocols: 2

**Last Updated:** November 2025
