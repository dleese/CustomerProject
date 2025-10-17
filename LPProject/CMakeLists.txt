cmake_minimum_required(VERSION 3.15)
project(LPProject VERSION 1.0 DESCRIPTION "Logipad initial Setup" LANGUAGES CXX)
string(TIMESTAMP LP_CURRENT_TIME_UTC "%Y-%m-%d %H:%M:%S" UTC)

include(cmake/GitVersion.cmake)

set(LP_CUSTOMER_COMPANY_NAME "Hungaria Airlines")
set(LP_CUSTOMER_DOMAIN "hua")

add_subdirectory("doc")
add_subdirectory("src")

