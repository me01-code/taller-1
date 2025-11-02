#!/bin/bash
# Installation script for NS-3 MANET simulations
# This script helps set up the simulations in an existing NS-3 installation

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Print colored messages
print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Banner
echo "================================================"
echo "NS-3 MANET Simulations Installation Script"
echo "================================================"
echo ""

# Check if NS-3 path is provided
if [ -z "$1" ]; then
    print_error "NS-3 path not provided!"
    echo ""
    echo "Usage: $0 /path/to/ns-3"
    echo ""
    echo "Example:"
    echo "  $0 ~/ns-allinone-3.38/ns-3.38"
    echo ""
    exit 1
fi

NS3_PATH="$1"

# Verify NS-3 installation
print_info "Checking NS-3 installation at: $NS3_PATH"

if [ ! -d "$NS3_PATH" ]; then
    print_error "Directory not found: $NS3_PATH"
    exit 1
fi

if [ ! -f "$NS3_PATH/waf" ]; then
    print_error "waf build tool not found. Is this a valid NS-3 directory?"
    exit 1
fi

print_info "✓ NS-3 installation verified"

# Create scratch directory if it doesn't exist
SCRATCH_DIR="$NS3_PATH/scratch"
if [ ! -d "$SCRATCH_DIR" ]; then
    print_info "Creating scratch directory..."
    mkdir -p "$SCRATCH_DIR"
fi

# Copy simulation files
print_info "Copying simulation files..."

SIM_FILES="manet-aodv.cc manet-olsr.cc"

for file in $SIM_FILES; do
    if [ -f "$file" ]; then
        print_info "  → $file"
        cp "$file" "$SCRATCH_DIR/"
    else
        print_warning "  ✗ $file not found, skipping"
    fi
done

# Build the simulations
print_info "Building simulations..."
cd "$NS3_PATH"

if ./waf build; then
    print_info "✓ Build successful!"
else
    print_error "Build failed. Check the error messages above."
    exit 1
fi

# Test the installation
print_info "Testing installation..."

if ./waf --run "scratch/manet-aodv --totalTime=1" > /dev/null 2>&1; then
    print_info "✓ AODV simulation test passed"
else
    print_warning "AODV simulation test failed"
fi

if ./waf --run "scratch/manet-olsr --totalTime=1" > /dev/null 2>&1; then
    print_info "✓ OLSR simulation test passed"
else
    print_warning "OLSR simulation test failed"
fi

# Check Python dependencies
print_info "Checking Python dependencies..."

if command -v python3 &> /dev/null; then
    print_info "✓ Python 3 found"
    
    if python3 -c "import matplotlib" 2> /dev/null; then
        print_info "✓ matplotlib installed"
    else
        print_warning "matplotlib not found (optional, needed for analysis)"
        print_info "Install with: pip3 install matplotlib"
    fi
else
    print_warning "Python 3 not found (optional, needed for helper scripts)"
fi

# Success message
echo ""
echo "================================================"
print_info "Installation Complete!"
echo "================================================"
echo ""
echo "Quick Start:"
echo "  cd $NS3_PATH"
echo "  ./waf --run manet-aodv"
echo "  ./waf --run manet-olsr"
echo ""
echo "With parameters:"
echo "  ./waf --run \"manet-aodv --nWifis=15 --totalTime=50\""
echo ""
echo "For more information, see:"
echo "  - README.md for detailed documentation"
echo "  - QUICKSTART.md for quick start guide"
echo "  - scenarios/ for pre-configured scenarios"
echo ""

exit 0
