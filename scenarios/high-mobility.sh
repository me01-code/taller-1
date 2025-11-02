#!/bin/bash
# High Mobility MANET Scenario
# Nodes move quickly with no pauses

echo "Running High Mobility Scenario..."
echo "================================="
echo "Configuration:"
echo "  - Nodes: 20"
echo "  - Speed: 20 m/s"
echo "  - Pause: 0 seconds"
echo "  - Duration: 150 seconds"
echo ""

# Run AODV
echo "Testing with AODV..."
./waf --run "manet-aodv --nWifis=20 --nodeSpeed=20 --nodePause=0 --totalTime=150"

echo ""
echo "Testing with OLSR..."
./waf --run "manet-olsr --nWifis=20 --nodeSpeed=20 --nodePause=0 --totalTime=150"

echo ""
echo "High Mobility Scenario Complete!"
