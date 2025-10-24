#!/bin/bash
#

echo "-- cd to build Directory"
cd ./build

echo "-- Clean Directory"
rm -fr *

cmake ../LPProject
# cmake -S ../LPProject -DCMAKE_BUILD_TYPE=Debug
cmake --build .
