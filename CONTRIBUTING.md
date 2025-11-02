# Contributing to NS-3 MANET Simulations

Thank you for your interest in contributing to this project! This document provides guidelines for contributing.

## How to Contribute

### Reporting Issues

If you find a bug or have a suggestion:

1. Check if the issue already exists in the issue tracker
2. If not, create a new issue with:
   - Clear description of the problem or suggestion
   - Steps to reproduce (for bugs)
   - Expected vs actual behavior
   - Your NS-3 version and environment details

### Submitting Changes

1. **Fork the repository**
   ```bash
   git clone https://github.com/me01-code/taller-1.git
   cd taller-1
   ```

2. **Create a feature branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```

3. **Make your changes**
   - Follow the coding style of existing files
   - Add comments where necessary
   - Test your changes thoroughly

4. **Commit your changes**
   ```bash
   git add .
   git commit -m "Brief description of changes"
   ```

5. **Push to your fork**
   ```bash
   git push origin feature/your-feature-name
   ```

6. **Create a Pull Request**
   - Provide a clear description of the changes
   - Reference any related issues
   - Explain testing performed

## Contribution Ideas

### New Routing Protocols

Add support for additional MANET routing protocols:
- **DSR (Dynamic Source Routing)**
- **DSDV (Destination-Sequenced Distance Vector)**
- **ZRP (Zone Routing Protocol)**
- **BATMAN (Better Approach To Mobile Ad-hoc Networking)**

Example structure:
```cpp
// manet-dsr.cc
#include "ns3/dsr-module.h"
// ... follow pattern from manet-aodv.cc
```

### New Scenarios

Create new pre-configured scenarios:
- Disaster recovery networks
- Smart city IoT networks
- Battlefield communications
- Aerial mesh networks (UAVs)

### Analysis Tools

Enhance the analysis capabilities:
- Add more visualization types
- Create comparison scripts for multiple runs
- Add statistical analysis (mean, variance, confidence intervals)
- Create animated visualizations of node movement

### Documentation

Improve documentation:
- Add more detailed explanations
- Create video tutorials
- Add troubleshooting guides
- Translate documentation to other languages

### Testing

Add test cases:
- Unit tests for simulation components
- Integration tests for complete scenarios
- Performance benchmarks

## Coding Guidelines

### C++ (NS-3 Simulations)

Follow NS-3 coding style:
- Use NS-3 naming conventions
- Include proper header comments
- Use NS_LOG for debugging output
- Follow GNU coding style for indentation

Example:
```cpp
/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("YourComponent");

void
YourFunction ()
{
  NS_LOG_FUNCTION (this);
  // Your code here
}
```

### Python Scripts

Follow PEP 8 style guide:
- Use 4 spaces for indentation
- Maximum line length: 100 characters
- Use meaningful variable names
- Add docstrings to functions

Example:
```python
#!/usr/bin/env python3
"""
Module description here
"""

def your_function(param1, param2):
    """
    Function description.
    
    Args:
        param1: Description
        param2: Description
    
    Returns:
        Description of return value
    """
    # Your code here
    pass
```

### Bash Scripts

- Use `#!/bin/bash` shebang
- Add comments explaining complex sections
- Check for errors and handle them gracefully
- Make scripts portable when possible

## Testing Your Changes

Before submitting:

1. **Build successfully**
   ```bash
   cd /path/to/ns-3
   ./waf build
   ```

2. **Run simulations**
   ```bash
   ./waf --run "manet-aodv --nWifis=5 --totalTime=30"
   ./waf --run "manet-olsr --nWifis=5 --totalTime=30"
   ```

3. **Test different parameters**
   - Test with various node counts
   - Test with different mobility patterns
   - Verify output makes sense

4. **Check Python scripts** (if modified)
   ```bash
   python3 -m pylint your_script.py
   python3 your_script.py --help
   ```

## Project Structure

When adding files, follow this structure:

```
taller-1/
├── manet-*.cc              # Main simulation files
├── *.py                    # Python helper scripts
├── scenarios/              # Pre-configured scenarios
│   ├── *.sh               # Scenario shell scripts
│   └── README.md          # Scenarios documentation
├── docs/                   # Additional documentation (if needed)
├── examples/               # Example outputs (if needed)
└── tests/                  # Test files (if added)
```

## Communication

- Use GitHub Issues for bug reports and feature requests
- Use Pull Requests for code contributions
- Be respectful and constructive in all interactions

## License

By contributing, you agree that your contributions will be licensed under the same license as the project (see LICENSE file).

## Questions?

If you have questions about contributing, feel free to:
- Open an issue with the "question" label
- Check existing documentation in README.md
- Review NS-3 documentation for NS-3-specific questions

## Recognition

Contributors will be acknowledged in the project. Significant contributions may be recognized in:
- README.md contributors section
- Release notes
- Project documentation

Thank you for contributing! 🎉
