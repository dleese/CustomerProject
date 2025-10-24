set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "" FORCE)

get_version_from_git()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Version.hpp.in Version.hpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/LPObject.hpp LPObject.hpp)

# Gather sources
set(BASE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Base/LPObject.cpp)
set(MAIN_SRC ${CMAKE_SOURCE_DIR}/main.cpp) # refers to root/main.cpp

add_executable(LPProject ${MAIN_SRC} ${BASE_SRC})
target_include_directories(LPProject PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# For multi-config generators, set output directory for all configurations
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set_target_properties(LPProject PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/src"
    )
endforeach()

