#!/bin/bash
# Dense Network MANET Scenario
# Many nodes in the simulation area

echo "Running Dense Network Scenario..."
echo "================================="
echo "Configuration:"
echo "  - Nodes: 30"
echo "  - Sinks: 10"
echo "  - Speed: 10 m/s"
echo "  - Duration: 200 seconds"
echo ""

# Run AODV
echo "Testing with AODV..."
./waf --run "manet-aodv --nWifis=30 --nSinks=10 --nodeSpeed=10 --totalTime=200"

echo ""
echo "Testing with OLSR..."
./waf --run "manet-olsr --nWifis=30 --nSinks=10 --nodeSpeed=10 --totalTime=200"

echo ""
echo "Dense Network Scenario Complete!"
