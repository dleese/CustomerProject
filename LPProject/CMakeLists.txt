cmake_minimum_required(VERSION 3.9)
project(LPProject VERSION 1.0 DESCRIPTION "Logipad initial Setup" LANGUAGES CXX)
string(TIMESTAMP LP_CURRENT_TIME_UTC "%Y-%m-%d %H:%M:%S" UTC)

include(cmake/GitVersion.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

set(LP_CUSTOMER_COMPANY_NAME "Hungaria Airlines")
set(LP_CUSTOMER_DOMAIN "hua")

# Specify the build output directory as build/src
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src)

# add_subdirectory("doc")
add_subdirectory("src")

