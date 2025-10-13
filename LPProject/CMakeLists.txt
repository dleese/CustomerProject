cmake_minimum_required(VERSION 3.15)
project(LPProject  VERSION 1.0 DESCRIPTION "Logipad initial Setup" LANGUAGES CXX)
string(TIMESTAMP LP_CURRENT_TIME_UTC "%Y-%m-%d %H:%M:%S" UTC)

include(cmake/GitVersion.cmake)
get_version_from_git()

set(LP_CUSTOMER_COMPANY_NAME "Empire Aviation Group")
set(LP_CUSTOMER_DOMAIN "empireaviationgroup")
set(LP_CUSTOMER_COMPANY_NAME "Empire Aviation")

add_subdirectory("doc")



