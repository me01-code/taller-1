# Makefile for NS-3 MANET Simulations
# This is a convenience wrapper around NS-3's waf build system

# Default NS-3 path (override with: make NS3_PATH=/your/path)
NS3_PATH ?= $(HOME)/ns-allinone-3.38/ns-3.38

# Simulation files
SIM_FILES = manet-aodv.cc manet-olsr.cc
SCRATCH_DIR = $(NS3_PATH)/scratch

.PHONY: help install build run-aodv run-olsr run-all clean check

help:
	@echo "NS-3 MANET Simulation Makefile"
	@echo "=============================="
	@echo ""
	@echo "Available targets:"
	@echo "  help       - Show this help message"
	@echo "  install    - Copy simulation files to NS-3 scratch directory"
	@echo "  build      - Build the simulations"
	@echo "  run-aodv   - Run AODV simulation"
	@echo "  run-olsr   - Run OLSR simulation"
	@echo "  run-all    - Run both simulations"
	@echo "  clean      - Clean build artifacts"
	@echo "  check      - Check NS-3 installation"
	@echo ""
	@echo "Usage examples:"
	@echo "  make install NS3_PATH=/path/to/ns-3"
	@echo "  make build"
	@echo "  make run-aodv ARGS='--nWifis=15 --totalTime=50'"
	@echo ""

check:
	@echo "Checking NS-3 installation..."
	@if [ -d "$(NS3_PATH)" ]; then \
		echo "✓ NS-3 found at: $(NS3_PATH)"; \
	else \
		echo "✗ NS-3 not found at: $(NS3_PATH)"; \
		echo "  Set correct path with: make NS3_PATH=/your/path"; \
		exit 1; \
	fi
	@if [ -f "$(NS3_PATH)/waf" ]; then \
		echo "✓ waf build tool found"; \
	else \
		echo "✗ waf not found in NS-3 directory"; \
		exit 1; \
	fi
	@echo "All checks passed!"

install: check
	@echo "Installing simulation files to NS-3..."
	@mkdir -p $(SCRATCH_DIR)
	@for file in $(SIM_FILES); do \
		echo "  Copying $$file..."; \
		cp -f $$file $(SCRATCH_DIR)/; \
	done
	@echo "✓ Installation complete!"

build: install
	@echo "Building simulations..."
	@cd $(NS3_PATH) && ./waf build
	@echo "✓ Build complete!"

run-aodv: build
	@echo "Running AODV simulation..."
	@cd $(NS3_PATH) && ./waf --run "manet-aodv $(ARGS)"

run-olsr: build
	@echo "Running OLSR simulation..."
	@cd $(NS3_PATH) && ./waf --run "manet-olsr $(ARGS)"

run-all: run-aodv
	@echo ""
	@make run-olsr ARGS="$(ARGS)"

clean:
	@echo "Cleaning build artifacts..."
	@cd $(NS3_PATH) && ./waf clean
	@echo "✓ Clean complete!"

# Scenario shortcuts
scenario-low-mobility: build
	@echo "Running low mobility scenario..."
	@cd $(NS3_PATH) && bash $(CURDIR)/scenarios/low-mobility.sh

scenario-high-mobility: build
	@echo "Running high mobility scenario..."
	@cd $(NS3_PATH) && bash $(CURDIR)/scenarios/high-mobility.sh

scenario-dense: build
	@echo "Running dense network scenario..."
	@cd $(NS3_PATH) && bash $(CURDIR)/scenarios/dense-network.sh

# Analysis
analyze:
	@echo "Analyzing results..."
	@if [ -f "flowmon.xml" ]; then \
		python3 analyze_results.py flowmon.xml; \
	else \
		echo "No flowmon.xml found. Run simulations first."; \
	fi
