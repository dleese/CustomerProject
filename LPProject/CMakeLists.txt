cmake_minimum_required(VERSION 3.15)

project(LPProject  VERSION 1.0 DESCRIPTION "Logipad initial Setup" LANGUAGES CXX)

string(TIMESTAMP LP_CURRENT_TIME_UTC "%Y-%m-%d %H:%M:%S" UTC)

set(LP_CUSTOMER_COMPANY_NAME "Empire Aviation Group")
set(LP_CUSTOMER_DOMAIN "empireaviationgroup")
set(LP_CUSTOMER_COMPANY_NAME} "Empire Aviation")

configure_file(EmpireNotes.md.in EmpireNotes.md)

# In a CMakeLists.txt file
include(cmake/GitVersion.cmake)

# Includes the file MyCustomFunctions.cmake
