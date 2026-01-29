#!/bin/bash

# Build script for Chess Game

set -e

echo "Building Chess Game..."

# Create build directory if it doesn't exist
mkdir -p build

# Navigate to build directory
cd build

# Run CMake
echo "Running CMake..."
cmake ..

# Build the project
echo "Building project..."
make -j$(nproc)

echo "Build completed successfully!"
echo "Run the game with: ./build/chess"
