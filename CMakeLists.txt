# Minimum CMake version required
cmake_minimum_required(VERSION 3.16)

# Project name
project(glt C CXX)

# --- Configuration ---
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# --- Find Dependencies ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLSLANG REQUIRED glslang)
pkg_check_modules(SPIRV_TOOLS REQUIRED SPIRV-Tools)

# --- Add Source Files ---
# Use file(GLOB_RECURSE ...) to automatically find all source files.

# C sources
file(GLOB_RECURSE C_SOURCES
    "main.c"            # <--- ADDED HERE
    "gl/core/*.c"
    "gl/shader/cache.c"
    "gles/*.c"
    "glx/*.c"
    "util/*.c"
)

# C++ sources (our new translator)
file(GLOB_RECURSE CXX_SOURCES
    "gl/shader/translate.cpp"
)

# --- Define the Shared Library Target ---
# This creates the libglt.so target from all C and C++ sources
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
)

# Add include directories found by pkg-config for glslang
target_include_directories(glt PRIVATE ${GLSLANG_INCLUDE_DIRS})

# Link against the libraries found by pkg-config
# Also link against libdl for dlsym/dlopen if your constructor uses it.
target_link_libraries(glt PRIVATE
    ${GLSLANG_LIBRARIES}
    ${SPIRV_TOOLS_LIBRARIES}
    dl
    spirv-cross-core
    spirv-cross-glsl
)

# Set compiler flags
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
message(STATUS "  glslang includes: ${GLSLANG_INCLUDE_DIRS}")
message(STATUS "  glslang libraries: ${GLSLANG_LIBRARIES}")
