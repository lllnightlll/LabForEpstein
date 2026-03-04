cmake_minimum_required(VERSION 3.16)

# -----------------------------
# Project configuration
# -----------------------------
set(PROJECT_NAME "LabForEpsteinI")  # Change if needed
project(${PROJECT_NAME} LANGUAGES CXX)

# -----------------------------
# Use Clang as C++ compiler
# -----------------------------
# If clang++ is in PATH this is enough.
# If you need a full path, set CMAKE_CXX_COMPILER in your CMake configure command:
#   cmake -DCMAKE_CXX_COMPILER="C:/Path/To/clang++.exe" ..
if(NOT CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER clang++ CACHE STRING "C++ compiler" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------
# Source files (recursive)
# -----------------------------
# Root directory for sources; you can change this or add more.
set(SRC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/LabForEpsteinI")

file(GLOB_RECURSE PROJECT_SOURCES
    CONFIGURE_DEPENDS
    "${SRC_ROOT}/*.cpp"
    "${SRC_ROOT}/*.cxx"
    "${SRC_ROOT}/*.cc"
)

if(PROJECT_SOURCES STREQUAL "")
    message(FATAL_ERROR "No source files found recursively under: ${SRC_ROOT}")
endif()

# -----------------------------
# Target
# -----------------------------
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)
