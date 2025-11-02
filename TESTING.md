# Testing Guide for NS-3 MANET Simulations

This guide helps you verify that your NS-3 MANET simulation setup is working correctly.

## Pre-Installation Testing

### 1. Verify NS-3 Installation

Before installing the MANET simulations, verify your NS-3 installation:

```bash
cd /path/to/ns-3.38

# Check NS-3 version
./waf --version

# Run a basic NS-3 example
./waf --run hello-simulator
```

**Expected Output:**
```
Hello Simulator
```

### 2. Check Required Modules

Verify that required NS-3 modules are available:

```bash
cd /path/to/ns-3.38

# List available modules
./waf --check

# Look for these modules:
# - aodv
# - olsr
# - wifi
# - mobility
# - applications
# - flow-monitor
```

## Installation Testing

### 1. Run Installation Script

```bash
./install.sh /path/to/ns-3.38
```

**Expected Output:**
```
[INFO] Checking NS-3 installation...
[INFO] ✓ NS-3 installation verified
[INFO] Copying simulation files...
[INFO] Building simulations...
[INFO] ✓ Build successful!
[INFO] Installation Complete!
```

### 2. Verify Files Copied

```bash
ls /path/to/ns-3.38/scratch/
```

**Expected Output:**
```
manet-aodv.cc
manet-olsr.cc
```

## Basic Functionality Tests

### Test 1: Quick AODV Simulation

Run a minimal AODV simulation:

```bash
cd /path/to/ns-3.38
./waf --run "manet-aodv --nWifis=5 --totalTime=10"
```

**Expected Output:**
```
Creating 5 nodes.
Creating devices and channels.
Installing Internet stack with AODV routing.
Setting up mobility.
Installing applications.
Run Simulation.
Flow 1 (10.1.1.x -> 10.1.1.y)
  Tx Packets: xxx
  Tx Bytes:   xxx
  Rx Packets: xxx
  Rx Bytes:   xxx
  Throughput: x.xx Mbps
```

**Success Criteria:**
- No error messages
- Flow statistics are displayed
- Throughput values are reasonable (> 0)

### Test 2: Quick OLSR Simulation

Run a minimal OLSR simulation:

```bash
cd /path/to/ns-3.38
./waf --run "manet-olsr --nWifis=5 --totalTime=10"
```

**Expected Output:**
Similar to AODV test with flow statistics.

**Success Criteria:**
- Completes without errors
- Similar output structure to AODV
- Non-zero throughput values

### Test 3: Parameter Variation

Test with different parameters:

```bash
./waf --run "manet-aodv --nWifis=15 --nodeSpeed=20 --totalTime=30"
```

**Success Criteria:**
- Simulation runs successfully
- More flows are reported (due to more nodes)
- Simulation takes approximately 30 seconds (real time)

## Python Scripts Testing

### Test 4: Python Runner Help

```bash
python3 run_manet_simulation.py --help
```

**Expected Output:**
```
usage: run_manet_simulation.py [-h] [--protocol {aodv,olsr,both}] ...
```

**Success Criteria:**
- No Python errors
- Help message displays all options

### Test 5: Analyzer Help

```bash
python3 analyze_results.py --help
```

**Expected Output:**
```
usage: analyze_results.py [-h] [--output OUTPUT] [--no-plot] xml_file
```

**Success Criteria:**
- No import errors
- Help message displays correctly

## Scenario Testing

### Test 6: Low Mobility Scenario

```bash
cd /path/to/ns-3.38
bash /path/to/taller-1/scenarios/low-mobility.sh
```

**Expected Output:**
- Runs both AODV and OLSR simulations
- Displays flow statistics for each
- No error messages

**Success Criteria:**
- Both protocols complete successfully
- PDR should be > 80% for both
- OLSR typically shows better performance in this scenario

### Test 7: High Mobility Scenario

```bash
bash /path/to/taller-1/scenarios/high-mobility.sh
```

**Success Criteria:**
- Both simulations complete
- AODV may show better PDR than OLSR
- Some packet loss is expected (70-90% PDR is normal)

### Test 8: Dense Network Scenario

```bash
bash /path/to/taller-1/scenarios/dense-network.sh
```

**Success Criteria:**
- Handles 30 nodes successfully
- Longer simulation time (200 seconds)
- More flows reported

## Performance Validation

### Test 9: Throughput Check

Run a simulation and check throughput values:

```bash
./waf --run "manet-aodv --nWifis=10 --totalTime=50 --rate=2048bps" | grep "Throughput"
```

**Success Criteria:**
- Throughput values should be < configured rate (2048bps = ~0.002 Mbps)
- Some flows may have 0 throughput (acceptable due to mobility)
- Average throughput across all flows should be > 0

### Test 10: Packet Delivery Ratio

Calculate PDR manually:

```bash
./waf --run "manet-aodv --nWifis=10 --totalTime=100" | tee test_output.txt
# Calculate: (Total Rx Packets / Total Tx Packets) * 100
```

**Success Criteria:**
- Low mobility: PDR > 80%
- Medium mobility: PDR > 70%
- High mobility: PDR > 60%

## Stress Testing

### Test 11: Large Network

Test scalability with many nodes:

```bash
./waf --run "manet-aodv --nWifis=40 --nSinks=15 --totalTime=100"
```

**Success Criteria:**
- Completes without crashing
- Takes longer to simulate
- May show lower PDR due to interference

### Test 12: Long Duration

Test with extended simulation time:

```bash
./waf --run "manet-olsr --nWifis=10 --totalTime=300"
```

**Success Criteria:**
- Runs for full duration
- Statistics remain reasonable
- No memory leaks or crashes

## Comparison Testing

### Test 13: Protocol Comparison

Run identical parameters with both protocols:

```bash
# AODV
./waf --run "manet-aodv --nWifis=15 --totalTime=100 --nodeSpeed=10" > aodv_results.txt

# OLSR
./waf --run "manet-olsr --nWifis=15 --totalTime=100 --nodeSpeed=10" > olsr_results.txt

# Compare
diff aodv_results.txt olsr_results.txt
```

**Success Criteria:**
- Both produce valid output
- Results differ (different routing behavior)
- Both have reasonable performance metrics

## Advanced Testing

### Test 14: Mobility Variations

Test different mobility patterns:

```bash
# Static (no movement)
./waf --run "manet-aodv --nodeSpeed=0"

# Low mobility
./waf --run "manet-aodv --nodeSpeed=5 --nodePause=10"

# High mobility
./waf --run "manet-aodv --nodeSpeed=30 --nodePause=0"
```

**Success Criteria:**
- Static: Highest PDR (~95-100%)
- Low mobility: Good PDR (~85-95%)
- High mobility: Moderate PDR (~70-85%)

### Test 15: Traffic Load Testing

Test different traffic rates:

```bash
# Light load
./waf --run "manet-aodv --rate=512bps"

# Medium load
./waf --run "manet-aodv --rate=2048bps"

# Heavy load
./waf --run "manet-aodv --rate=8192bps"
```

**Success Criteria:**
- Higher traffic = lower PDR (expected)
- Throughput increases with rate (up to network capacity)
- No crashes at any load level

## Troubleshooting Tests

### Issue: No Output

**Test:**
```bash
./waf --run "manet-aodv --nWifis=10 --totalTime=10"
```

**If no output appears:**
1. Check NS-3 build: `./waf build`
2. Check for errors: `./waf --run manet-aodv 2>&1 | grep -i error`
3. Verify file exists: `ls scratch/manet-aodv.cc`

### Issue: Build Failures

**Test:**
```bash
cd /path/to/ns-3.38
./waf clean
./waf configure --enable-examples
./waf build
```

**Check for:**
- Compiler errors
- Missing dependencies
- Module availability

### Issue: Low Performance

**Test:**
```bash
./waf --run "manet-aodv --nWifis=5 --totalTime=50"
```

**If PDR < 50%:**
1. Increase simulation time
2. Reduce node speed
3. Reduce number of nodes
4. Adjust traffic rate

## Test Results Documentation

Create a test log:

```bash
#!/bin/bash
# Save as test_all.sh

echo "NS-3 MANET Simulation Test Report" > test_report.txt
echo "==================================" >> test_report.txt
echo "Date: $(date)" >> test_report.txt
echo "" >> test_report.txt

echo "Test 1: AODV Quick Test" >> test_report.txt
./waf --run "manet-aodv --nWifis=5 --totalTime=10" >> test_report.txt 2>&1
echo "" >> test_report.txt

echo "Test 2: OLSR Quick Test" >> test_report.txt
./waf --run "manet-olsr --nWifis=5 --totalTime=10" >> test_report.txt 2>&1
echo "" >> test_report.txt

echo "All tests completed. See test_report.txt for details."
```

## Performance Benchmarks

Expected performance ranges (approximate):

| Scenario | AODV PDR | OLSR PDR | Avg Delay | Notes |
|----------|----------|----------|-----------|-------|
| Static (0 m/s) | 95-100% | 95-100% | 5-20ms | Best case |
| Low mobility (5 m/s) | 85-95% | 90-98% | 10-30ms | Stable |
| Medium mobility (10 m/s) | 75-85% | 80-90% | 15-50ms | Good |
| High mobility (20 m/s) | 65-80% | 60-75% | 20-80ms | Challenging |
| Very high (30+ m/s) | 50-70% | 45-65% | 30-100ms | Stress test |

## Continuous Testing

For development or research, create automated tests:

```bash
#!/bin/bash
# Automated daily test

for protocol in aodv olsr; do
    for nodes in 5 10 15 20; do
        echo "Testing $protocol with $nodes nodes..."
        ./waf --run "manet-$protocol --nWifis=$nodes --totalTime=50" > \
            test_${protocol}_${nodes}nodes_$(date +%Y%m%d).txt
    done
done
```

## Validation Checklist

Before considering your installation complete:

- [ ] NS-3 builds without errors
- [ ] Both AODV and OLSR simulations run
- [ ] Flow statistics are displayed
- [ ] PDR values are reasonable (> 60%)
- [ ] Python scripts run without errors
- [ ] Scenarios execute successfully
- [ ] Parameter variations work
- [ ] Documentation is accessible
- [ ] Help commands work
- [ ] No persistent error messages

## Getting Help

If tests fail:

1. **Check NS-3 Version:** Ensure you're using NS-3 3.35 or later
2. **Review Error Messages:** Look for specific module or compilation errors
3. **Verify Installation:** Re-run `./install.sh`
4. **Check Documentation:** Review README.md and QUICKSTART.md
5. **Report Issues:** Create a GitHub issue with test results

## Test Success Criteria Summary

Your installation is successful if:

✅ All 15 basic tests pass
✅ Both protocols produce output
✅ PDR values are within expected ranges
✅ Python scripts execute without errors
✅ Scenarios run to completion
✅ Performance metrics are reasonable
✅ No persistent error messages

---

**Note:** Some variation in results is normal due to the random nature of mobility and traffic patterns. Run tests multiple times for statistical confidence.
