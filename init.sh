#!/bin/sh
set -e

# Build directory
BUILD_DIR="./build"

# Recreate build directory
rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# Create build system
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

# Build targets
make lineage
