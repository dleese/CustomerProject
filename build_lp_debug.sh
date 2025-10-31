#!/bin/bash
#
# (c) Dirk Leese 26.10.2025
#
# assume we are in the current Project directory which is 'LPProject'
#

# does build/ directory exits
# YES => delete directory
#
if [ -d ./build ]; then
  echo "Found build directory..."
  echo "Remove build Directory!"
  rm -fr ./build/
fi
mkdir build && cd build
cmake ../LPProject -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target LPProject
#cmake --build . --target doxygen_docs
