# Minimum CMake version required
cmake_minimum_required(VERSION 3.16)

# Project name
project(glt C CXX)

# --- Configuration ---
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# --- Find Dependencies ---
find_package(glslang REQUIRED)
find_package(spirv_cross_core REQUIRED)
find_package(spirv_cross_glsl REQUIRED)
# The X11 library is required for the Android shim
find_package(X11 REQUIRED)

# --- Add Source Files ---
# Use file(GLOB_RECURSE ...) to automatically find all source files.

# C sources
file(GLOB_RECURSE C_SOURCES
    "main.c"
    "gl/core/*.c"
    "gl/shader/cache.c"
    "gles/*.c"
    "glx/glx.c"
    "util/*.c"
)

# C++ sources
file(GLOB_RECURSE CXX_SOURCES
    "gl/shader/translate.cpp"
)

# --- Define the Shared Library Target ---
add_library(glt SHARED ${C_SOURCES} ${CXX_SOURCES})

# --- Target Properties and Includes ---
# Set properties for our 'glt' library target

# Add include directories for our project structure
target_include_directories(glt PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/gles"
    "${CMAKE_CURRENT_SOURCE_DIR}/gl/core"
    "${CMAKE_CURRENT_SOURCE_DIR}/gl/shader"
    "${CMAKE_CURRENT_SOURCE_DIR}/util"
    "${CMAKE_CURRENT_SOURCE_DIR}/glx"
)

# Add include directories from dependencies
target_include_directories(glt PRIVATE
    ${GLSLANG_INCLUDE_DIRS}
    ${SPIRV-Cross_INCLUDE_DIRS}
)

# Link against required libraries
target_link_libraries(glt PRIVATE
    dl
    # Use modern imported targets if available, otherwise fall back to variables
    glslang::glslang
    spirv-cross-core
    spirv-cross-glsl
    ${X11_LIBRARIES} # Link against X11 for the shim
)

# Set common compiler flags
target_compile_options(glt PRIVATE
    -fPIC
    -Wall
    -g # For debugging
    -O2 # Optimization level
)


# --- Final Output ---
# Print a status message
message(STATUS "Configured glt shared library.")
message(STATUS "  C sources: ${C_SOURCES}")
message(STATUS "  C++ sources: ${CXX_SOURCES}")
