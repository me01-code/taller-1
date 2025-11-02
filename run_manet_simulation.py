#!/usr/bin/env python3
"""
NS-3 MANET Simulation Runner
This script helps run and analyze MANET simulations with different routing protocols
"""

import subprocess
import sys
import os
import argparse

def check_ns3_installation():
    """Check if NS-3 is properly installed"""
    import glob
    
    ns3_paths = [
        "/usr/local/ns-3",
        "/opt/ns-3",
        os.path.expanduser("~/ns-3"),
    ]
    
    # Add paths from glob patterns
    ns3_paths.extend(glob.glob(os.path.expanduser("~/ns-allinone-3.*/ns-3.*")))
    
    print("Checking for NS-3 installation...")
    for path in ns3_paths:
        if os.path.exists(path):
            print(f"Found NS-3 at: {path}")
            return path
    
    print("NS-3 not found in common locations.")
    print("Please ensure NS-3 is installed.")
    print("Visit: https://www.nsnam.org/releases/")
    return None

def build_simulation(ns3_path, protocol):
    """Build the simulation using waf"""
    print(f"\nBuilding MANET {protocol.upper()} simulation...")
    
    src_file = f"manet-{protocol}.cc"
    dest_file = os.path.join(ns3_path, "scratch", src_file)
    
    # Copy simulation file to NS-3 scratch directory
    if os.path.exists(src_file):
        os.makedirs(os.path.join(ns3_path, "scratch"), exist_ok=True)
        subprocess.run(["cp", src_file, dest_file])
        print(f"Copied {src_file} to NS-3 scratch directory")
    else:
        print(f"Error: {src_file} not found")
        return False
    
    # Build with waf
    os.chdir(ns3_path)
    result = subprocess.run(["./waf", "build"], capture_output=True, text=True)
    
    if result.returncode == 0:
        print("Build successful!")
        return True
    else:
        print("Build failed:")
        print(result.stderr)
        return False

def run_simulation(ns3_path, protocol, params):
    """Run the simulation with specified parameters"""
    print(f"\nRunning MANET {protocol.upper()} simulation...")
    
    # Build waf command with parameters
    run_args = f"manet-{protocol}"
    
    # Add command line parameters
    param_parts = []
    if params.get('nWifis'):
        param_parts.append(f"--nWifis={params['nWifis']}")
    if params.get('nSinks'):
        param_parts.append(f"--nSinks={params['nSinks']}")
    if params.get('totalTime'):
        param_parts.append(f"--totalTime={params['totalTime']}")
    if params.get('rate'):
        param_parts.append(f"--rate={params['rate']}")
    if params.get('nodeSpeed'):
        param_parts.append(f"--nodeSpeed={params['nodeSpeed']}")
    
    if param_parts:
        run_args += " " + " ".join(param_parts)
    
    cmd = ["./waf", "--run", run_args]
    
    os.chdir(ns3_path)
    result = subprocess.run(cmd, capture_output=True, text=True)
    
    print("\n=== Simulation Output ===")
    print(result.stdout)
    
    if result.returncode != 0:
        print("\n=== Simulation Errors ===")
        print(result.stderr)
        return False
    
    return True

def main():
    parser = argparse.ArgumentParser(description='Run NS-3 MANET simulations')
    parser.add_argument('--protocol', type=str, choices=['aodv', 'olsr', 'both'],
                        default='both', help='Routing protocol to use')
    parser.add_argument('--nWifis', type=int, default=10,
                        help='Number of WiFi nodes (default: 10)')
    parser.add_argument('--nSinks', type=int, default=3,
                        help='Number of sink nodes (default: 3)')
    parser.add_argument('--totalTime', type=float, default=100.0,
                        help='Simulation time in seconds (default: 100)')
    parser.add_argument('--rate', type=str, default='2048bps',
                        help='Traffic rate (default: 2048bps)')
    parser.add_argument('--nodeSpeed', type=int, default=10,
                        help='Node speed in m/s (default: 10)')
    parser.add_argument('--ns3-path', type=str,
                        help='Path to NS-3 installation')
    parser.add_argument('--build-only', action='store_true',
                        help='Only build, do not run')
    
    args = parser.parse_args()
    
    # Check NS-3 installation
    ns3_path = args.ns3_path if args.ns3_path else check_ns3_installation()
    
    if not ns3_path:
        print("\nPlease specify NS-3 path with --ns3-path option")
        return 1
    
    # Prepare simulation parameters
    params = {
        'nWifis': args.nWifis,
        'nSinks': args.nSinks,
        'totalTime': args.totalTime,
        'rate': args.rate,
        'nodeSpeed': args.nodeSpeed
    }
    
    # Determine which protocols to run
    protocols = []
    if args.protocol == 'both':
        protocols = ['aodv', 'olsr']
    else:
        protocols = [args.protocol]
    
    # Build and run simulations
    for protocol in protocols:
        print(f"\n{'='*60}")
        print(f"Processing {protocol.upper()} simulation")
        print(f"{'='*60}")
        
        if not build_simulation(ns3_path, protocol):
            print(f"Failed to build {protocol} simulation")
            continue
        
        if not args.build_only:
            if not run_simulation(ns3_path, protocol, params):
                print(f"Failed to run {protocol} simulation")
    
    print("\n" + "="*60)
    print("All simulations completed!")
    print("="*60)
    
    return 0

if __name__ == "__main__":
    sys.exit(main())
