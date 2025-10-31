cmake_minimum_required(VERSION 3.18)
project(LPProject VERSION 1.0 DESCRIPTION "Logipad initial Setup" LANGUAGES CXX)
string(TIMESTAMP LP_CURRENT_TIME_UTC "%Y-%m-%d %H:%M:%S" UTC)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(LP_CUSTOMER_COMPANY_NAME "Hungaria Airlines")
set(LP_CUSTOMER_DOMAIN "hua")

include(GitVersion)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(doc)

include(Doxygen)
Doxygen(src doxygen)

