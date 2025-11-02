#!/bin/bash
# Low Mobility MANET Scenario
# Nodes move slowly with frequent pauses

echo "Running Low Mobility Scenario..."
echo "================================"
echo "Configuration:"
echo "  - Nodes: 15"
echo "  - Speed: 5 m/s"
echo "  - Pause: 10 seconds"
echo "  - Duration: 150 seconds"
echo ""

# Run AODV
echo "Testing with AODV..."
./waf --run "manet-aodv --nWifis=15 --nodeSpeed=5 --nodePause=10 --totalTime=150"

echo ""
echo "Testing with OLSR..."
./waf --run "manet-olsr --nWifis=15 --nodeSpeed=5 --nodePause=10 --totalTime=150"

echo ""
echo "Low Mobility Scenario Complete!"
