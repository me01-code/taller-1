# MANET Simulation Scenarios

This directory contains pre-configured scenarios for testing different MANET conditions.

## Available Scenarios

### 1. Low Mobility (`low-mobility.sh`)
Simulates a scenario where nodes move slowly and pause frequently.

**Characteristics:**
- 15 nodes
- Node speed: 5 m/s
- Pause time: 10 seconds
- Simulation duration: 150 seconds

**Use Case:** 
- Pedestrian networks
- Slow-moving vehicular networks in congested areas
- IoT sensor networks with limited mobility

### 2. High Mobility (`high-mobility.sh`)
Simulates a scenario where nodes move quickly without pausing.

**Characteristics:**
- 20 nodes
- Node speed: 20 m/s
- Pause time: 0 seconds
- Simulation duration: 150 seconds

**Use Case:**
- High-speed vehicular networks
- UAV/drone networks
- Emergency response scenarios

### 3. Dense Network (`dense-network.sh`)
Simulates a scenario with many nodes in the simulation area.

**Characteristics:**
- 30 nodes
- 10 sink nodes
- Node speed: 10 m/s
- Simulation duration: 200 seconds

**Use Case:**
- Urban environments
- Large-scale events (concerts, stadiums)
- Military tactical networks

## Running Scenarios

### Prerequisites
1. Copy the simulation files to NS-3 scratch directory
2. Build NS-3 with the simulation files
3. Navigate to NS-3 root directory

### Execution

```bash
# Make scripts executable
chmod +x scenarios/*.sh

# Run from NS-3 root directory
cd /path/to/ns-3.38

# Execute a scenario
bash /path/to/scenarios/low-mobility.sh
```

Or individually:

```bash
cd /path/to/ns-3.38
bash /path/to/taller-1/scenarios/low-mobility.sh
```

## Creating Custom Scenarios

To create your own scenario:

1. Copy one of the existing scenario scripts
2. Modify the parameters:
   - `--nWifis`: Number of nodes
   - `--nSinks`: Number of receiver nodes
   - `--nodeSpeed`: Maximum node speed in m/s
   - `--nodePause`: Pause time between movements
   - `--totalTime`: Simulation duration
   - `--rate`: Traffic generation rate

3. Save and make executable:
   ```bash
   chmod +x your-scenario.sh
   ```

## Expected Outcomes

### Low Mobility Scenario
- **OLSR**: Should perform better due to stable topology
- **AODV**: May have slightly higher overhead but still efficient
- Higher packet delivery ratio
- Lower end-to-end delay

### High Mobility Scenario
- **AODV**: Better adaptation to topology changes
- **OLSR**: May experience more route inconsistencies
- More frequent route breaks
- Higher routing overhead

### Dense Network Scenario
- **OLSR**: May have higher control overhead
- **AODV**: Better scalability
- More interference and contention
- Testing scalability of protocols

## Analyzing Results

After running scenarios, analyze the results:

```bash
# View console output for immediate statistics
# Check generated .pcap files for detailed packet analysis
# Use analyze_results.py for visualization (if XML output is enabled)
```

## Tips

1. **Multiple Runs**: Run each scenario multiple times with different random seeds for statistical significance
2. **Parameter Sweep**: Vary one parameter at a time to understand its impact
3. **Comparison**: Run both protocols with identical parameters for fair comparison
4. **Documentation**: Document your findings for each scenario
