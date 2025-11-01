get_version_from_git()

# Configure header template files
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Version.hpp.in 
  ${CMAKE_BINARY_DIR}/src/Version.hpp
)

# Note: LPKeyCloakClient.hpp doesn't need configure_file as it has no template variables
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/LPKeyCloakClient.hpp ${CMAKE_BINARY_DIR}/src/LPKeyCloakClient.hpp)

include(cpp-httplib)
CppHttpLib()

# Source files
set(SOURCES
  main.cpp
  Base/LPHelperObject.cpp
  Base/LPKeyCloakClient.cpp
  Base/LPLogipadClient.cpp
)

# Find dependencies
find_package(nlohmann_json CONFIG REQUIRED)

# Create executable target
add_executable(LPProject ${SOURCES})

# Include directories
target_include_directories(LPProject PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_BINARY_DIR}/src
)

# Link libraries - nlohmann_json interface includes are handled automatically
target_link_libraries(LPProject PRIVATE 
  httplib 
  #nlohmann_json::nlohmann_json
)

# Set output directory (already set in root, but can be overridden per target if needed)
# set_target_properties(LPProject PROPERTIES
#   RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
# )
