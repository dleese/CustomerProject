get_version_from_git()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/Version.hpp.in ${CMAKE_BINARY_DIR}/src/Version.hpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/LPKeyCloakClient.hpp ${CMAKE_BINARY_DIR}/src/LPKeyCloakClient.hpp)

include(cpp-httplib)

CppHttpLib()

set(SOURCES
  main.cpp
  Base/LPHelperObject.cpp
  Base/LPKeyCloakClient.cpp
  Base/LPLogipadClient.cpp
)
find_package(nlohmann_json CONFIG REQUIRED)
add_executable(LPProject ${SOURCES})

target_include_directories(LPProject PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_BINARY_DIR}/src
  nlohmann_json::nlohmann_json
)
target_link_libraries(LPProject PRIVATE httplib)
