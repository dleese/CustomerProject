#!/bin/bash
#
# Build script for LPProject
# (c) Dirk Leese 2025
#
# Usage:
#   ./build_lp_debug.sh [OPTIONS]
#
# Options:
#   -d, --debug       Build in Debug mode (default)
#   -r, --release     Build in Release mode
#   -D, --docs-only   Build only Doxygen documentation (no compilation)
#   -h, --help        Show this help message
#
# Examples:
#   ./build_lp_debug.sh              # Debug build (default)
#   ./build_lp_debug.sh --debug      # Explicit debug build
#   ./build_lp_debug.sh --release    # Release build
#   ./build_lp_debug.sh --docs-only  # Only generate documentation
#

set -e  # Exit on error

# Default values
BUILD_TYPE="Debug"
BUILD_TARGET="LPProject"
DOCS_ONLY=false
PROJECT_DIR="LPProject"
BUILD_DIR="build"

# Function to display help
show_help() {
    cat << EOF
Build script for LPProject

Usage:
    $0 [OPTIONS]

Options:
    -d, --debug       Build in Debug mode (default)
    -r, --release     Build in Release mode
    -D, --docs-only   Build only Doxygen documentation (no compilation)
    -h, --help        Show this help message

Examples:
    $0                    # Debug build (default)
    $0 --debug            # Explicit debug build
    $0 --release          # Release build
    $0 --docs-only        # Only generate documentation
    $0 --release --docs   # Release build with documentation
EOF
}

# Parse command-line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--debug)
            BUILD_TYPE="Debug"
            BUILD_TARGET="LPProject"
            DOCS_ONLY=false
            shift
            ;;
        -r|--release)
            BUILD_TYPE="Release"
            BUILD_TARGET="LPProject"
            DOCS_ONLY=false
            shift
            ;;
        -D|--docs-only)
            DOCS_ONLY=true
            BUILD_TARGET="doxygen_docs"
            shift
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Check if we're in the correct directory
if [ ! -d "$PROJECT_DIR" ]; then
    echo "Error: $PROJECT_DIR directory not found!"
    echo "Please run this script from the project root directory."
    exit 1
fi

# Remove existing build directory if it exists
if [ -d "$BUILD_DIR" ]; then
    echo "Found existing build directory..."
    echo "Removing build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create build directory
echo "Creating build directory..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure CMake
echo "Configuring CMake for $BUILD_TYPE build..."
cmake "../$PROJECT_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

# Build targets
if [ "$DOCS_ONLY" = true ]; then
    echo "Building Doxygen documentation only..."
    cmake --build . --target doxygen_docs
    echo "Documentation build complete!"
    echo "Documentation location: $(pwd)/doxygen/html/index.html"
else
    echo "Building $BUILD_TARGET in $BUILD_TYPE mode..."
    cmake --build . --target "$BUILD_TARGET"
    
    # Optionally build documentation as well
    if command -v doxygen >/dev/null 2>&1; then
        echo "Building documentation..."
        cmake --build . --target doxygen_docs 2>/dev/null || echo "Documentation build skipped (doxygen may not be configured)"
    fi
    
    echo "Build complete!"
    echo "Executable location: $(pwd)/bin/$BUILD_TARGET"
fi

cd ..
echo "Done!"
