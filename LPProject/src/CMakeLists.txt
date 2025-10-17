add_executable (LPProject main.cpp)
configure_file (Version.h.in Version.h)
target_include_directories(LPProject PUBLIC "${PROJECT_BINARY_DIR}/src/")
