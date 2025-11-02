# Quick Start Guide - NS-3 MANET Simulations

This guide will help you get started with the MANET simulations quickly.

## 5-Minute Quick Start

### Step 1: Install NS-3 (if not already installed)

```bash
# Download NS-3
wget https://www.nsnam.org/releases/ns-allinone-3.38.tar.bz2
tar xjf ns-allinone-3.38.tar.bz2
cd ns-allinone-3.38

# Build NS-3
./build.py --enable-examples
```

### Step 2: Copy Simulation Files

```bash
# From this repository directory
NS3_PATH=/path/to/ns-allinone-3.38/ns-3.38

# Copy simulation files
cp manet-aodv.cc $NS3_PATH/scratch/
cp manet-olsr.cc $NS3_PATH/scratch/
```

### Step 3: Build Simulations

```bash
cd $NS3_PATH
./waf build
```

### Step 4: Run Your First Simulation

```bash
# Run AODV simulation (default parameters)
./waf --run manet-aodv

# Or with custom parameters
./waf --run "manet-aodv --nWifis=15 --totalTime=50"
```

## What You'll See

When you run a simulation, you'll see output like:

```
Flow 1 (10.1.1.8 -> 10.1.1.1)
  Tx Packets: 12345
  Tx Bytes:   12640768
  Rx Packets: 11234
  Rx Bytes:   11503616
  Throughput: 0.92 Mbps

Flow 2 (10.1.1.9 -> 10.1.1.2)
  ...
```

## Understanding the Output

- **Tx Packets/Bytes**: Data sent from source
- **Rx Packets/Bytes**: Data received at destination
- **Throughput**: Achieved data rate
- **Packet Delivery Ratio**: (Rx Packets / Tx Packets) × 100%

## Running Different Scenarios

### Small Network Test
```bash
./waf --run "manet-aodv --nWifis=5 --totalTime=30"
```

### High Mobility Test
```bash
./waf --run "manet-aodv --nodeSpeed=20 --nodePause=0"
```

### Protocol Comparison
```bash
# Run AODV
./waf --run "manet-aodv --nWifis=10 --totalTime=100" > aodv_results.txt

# Run OLSR with same parameters
./waf --run "manet-olsr --nWifis=10 --totalTime=100" > olsr_results.txt

# Compare results
diff aodv_results.txt olsr_results.txt
```

## Using Pre-configured Scenarios

```bash
# Make scenario scripts executable
chmod +x scenarios/*.sh

# Run a scenario (from NS-3 directory)
bash /path/to/taller-1/scenarios/low-mobility.sh
```

## Analyzing Results with Python

If you have matplotlib installed:

```bash
# Install dependencies
pip3 install matplotlib numpy

# Run analysis (if FlowMonitor XML was generated)
python3 analyze_results.py flowmon.xml
```

## Common Parameters Cheat Sheet

| Parameter | Description | Example Values |
|-----------|-------------|----------------|
| `--nWifis` | Total nodes | 5, 10, 20, 30 |
| `--nSinks` | Receiver nodes | 2, 3, 5 |
| `--totalTime` | Simulation duration (s) | 30, 100, 200 |
| `--nodeSpeed` | Max speed (m/s) | 5, 10, 20 |
| `--nodePause` | Pause duration (s) | 0, 5, 10 |
| `--rate` | Traffic rate | "1024bps", "2048bps" |

## Troubleshooting

### "Command not found: waf"
- Make sure you're in the NS-3 directory
- Try: `./waf --version` to verify

### "Module not found" errors
- Rebuild NS-3: `./waf clean && ./waf configure && ./waf build`
- Check that NS-3 modules are enabled

### No output/silent failure
- Check that nWifis > nSinks
- Increase totalTime parameter
- Check NS-3 logs: `NS_LOG=ManetAodvExample=level_info ./waf --run manet-aodv`

## Next Steps

1. Read the full [README.md](README.md) for detailed documentation
2. Explore the [scenarios/](scenarios/) directory for more examples
3. Modify simulation parameters to test different conditions
4. Create custom scenarios for your specific use case
5. Compare AODV vs OLSR performance metrics

## Getting Help

- NS-3 Documentation: https://www.nsnam.org/documentation/
- NS-3 Users Mailing List: https://groups.google.com/g/ns-3-users
- This project's README.md for detailed information

Happy simulating! 🚀
