cmake_minimum_required(VERSION 3.20)
project(PolymorphismBenchmark CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===========================
# BUILD CONFIGURATION OPTIONS
# ===========================

option(RESET_DEFAULTS "Reset all options to default settings" OFF)
option(ENABLE_DEBUG "Enable Debugging Symbols (-O0 -g)" OFF)
option(ENABLE_NO_INLINE "Disable Function Inlining (-fno-inline)" OFF)
option(ENABLE_LOW_OPT "Use Lower Optimization (-O1 instead of -O3)" OFF)
option(ENABLE_PROFILING "Enable Profiling (-pg, for gprof/perf)" OFF)
option(ENABLE_CONCEPT_ERROR_DETAIL "Enable Verbose Compiler Errors for Concepts" ON)

# ===========================
# HANDLE RESET_DEFAULTS OPTION
# ===========================

if (RESET_DEFAULTS)
    message(STATUS "Resetting all options to default settings (turning them OFF)...")
    
    # Reset options to their default OFF state
    set(ENABLE_DEBUG OFF CACHE BOOL "Disable Debugging Symbols (Reset to Default)" FORCE)
    set(ENABLE_NO_INLINE OFF CACHE BOOL "Allow Function Inlining (Reset to Default)" FORCE)
    set(ENABLE_LOW_OPT OFF CACHE BOOL "Use Default Optimization (-O3, Reset)" FORCE)
    set(ENABLE_PROFILING OFF CACHE BOOL "Disable Profiling (Reset to Default)" FORCE)
endif()

# ===========================
# DEFAULT COMPILER FLAGS (Handled Safely)
# ===========================

if (ENABLE_DEBUG)
    set(CMAKE_CXX_FLAGS "-O0 -g")
elseif (ENABLE_NO_INLINE)
    set(CMAKE_CXX_FLAGS "-O3 -march=native -fno-inline")
elseif (ENABLE_LOW_OPT)
    set(CMAKE_CXX_FLAGS "-O1 -march=native")
elseif (ENABLE_PROFILING)
    set(CMAKE_CXX_FLAGS "-O3 -march=native -pg")
else()
    # Default optimized build (Ensures no empty flags!)
    set(CMAKE_CXX_FLAGS "-O3 -march=native")
endif()

# Enable verbose error messages for C++ Concepts (if using GCC)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND ENABLE_CONCEPT_ERROR_DETAIL)
    add_compile_options(-fconcepts-diagnostics-depth=5)
endif()

message(STATUS "Building with flags: ${CMAKE_CXX_FLAGS}")

# ===========================
# INCLUDE DIRECTORIES
# ===========================

include_directories(include)

# ===========================
# LOCATION OF EXECUTABLE
# ===========================

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ===========================
# SOURCE FILES
# ===========================

set(SRC_FILES
    src/main.cpp
    src/cli_utils.cpp
    src/benchmark_utils.cpp
    src/runtime_polymorphism.cpp
    src/crtp_polymorphism.cpp
    src/concepts_polymorphism.cpp
    src/polymorphism_tests.cpp
    src/test_runner.cpp
)

# ===========================
# ADD DEFINITIONS
# ===========================

add_definitions(-DCOMPILER_FLAGS="${CMAKE_CXX_FLAGS}")


# ===========================
# BUILD TARGET
# ===========================

add_executable(benchmark ${SRC_FILES})
