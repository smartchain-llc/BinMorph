#!/bin/bash
# Navigate to build directory
cd build

# Clean the build
cmake --build . --target clean

# Rebuild the project
cmake --build .

# Navigate back to project root
cd ..

# Run Doxygen
doxygen documentation/Doxyfile
