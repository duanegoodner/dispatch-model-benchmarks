cmake_minimum_required(VERSION 3.20)
project(PolymorphismBenchmark CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===========================
# LOCATION OF EXECUTABLE
# ===========================

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ===========================
# BUILD CONFIGURATION OPTIONS
# ===========================

option(RESET_DEFAULTS "Reset all options to default settings" OFF)
option(ENABLE_O0 "No optimization")
option(ENABLE_O1 "Use Lower Optimization (-O1 instead of -O3)" OFF)
option(ENABLE_O2 "Use Moderate Optimization (-O2 instead of -O3)" OFF)
option(ENABLE_O3 "Use Maximum Optimization (-O3, Default)" OFF)
option(ENABLE_DEBUG "Enable Debugging Symbols (-O0 -g)" OFF)
option(ENABLE_NO_INLINE "Disable Function Inlining (-fno-inline)" OFF)
option(ENABLE_PROFILING "Enable Profiling (-pg, for gprof/perf)" OFF)
option(ENABLE_CONCEPT_ERROR_DETAIL "Enable Verbose Compiler Errors for Concepts" ON)

# ===========================
# HANDLE RESET_DEFAULTS OPTION
# ===========================

if (RESET_DEFAULTS)
    message(STATUS "Resetting all options to default settings (turning them OFF)...")
    set(ENABLE_DEBUG OFF CACHE BOOL "Disable Debugging Symbols (Reset to Default)" FORCE)
    set(ENABLE_O0 OFF CACHE BOOL "Disable -O0 Optimization (Reset to Default)" FORCE)
    set(ENABLE_O1 OFF CACHE BOOL "Disable -O1 Optimization (Reset to Default)" FORCE)
    set(ENABLE_O2 OFF CACHE BOOL "Disable -O2 Optimization (Reset to Default)" FORCE)
    set(ENABLE_O3 ON CACHE BOOL "Enable -O3 Optimization (Default)" FORCE)
    set(ENABLE_NO_INLINE OFF CACHE BOOL "Allow Function Inlining (Reset to Default)" FORCE)
    set(ENABLE_PROFILING OFF CACHE BOOL "Disable Profiling (Reset to Default)" FORCE)
    set(ENABLE_CONCEPT_ERROR_DETAIL ON CACHE BOOL "Enable Verbose Compiler Errors for Concepts (Default)" FORCE)
endif()


# ===========================
# ENSURE AT MOST ONE OPTIMIZATION LEVEL IS SET
# ===========================

# Convert ON/OFF values to 1/0 manually
set(VALUE_DEBUG 0)
set(VALUE_O0 0)
set(VALUE_O1 0)
set(VALUE_O2 0)
set(VALUE_O3 0)

if(ENABLE_DEBUG)
    set(VALUE_DEBUG 1)
endif()
if(ENABLE_O0)
    set(VALUE_O0 0)
endif()
if(ENABLE_O1)
    set(VALUE_O1 1)
endif()
if(ENABLE_O2)
    set(VALUE_O2 1)
endif()
if(ENABLE_O3)
    set(VALUE_O3 1)
endif()

# Compute the sum of enabled options
math(EXPR COUNT_ENABLED_OPTIMIZATION_LEVELS "${VALUE_DEBUG} + ${VALUE_O0} + ${VALUE_O1} + ${VALUE_O2} + ${VALUE_O3}")

# Check if more than one option is enabled
if(COUNT_ENABLED_OPTIMIZATION_LEVELS GREATER 1)
    message(FATAL_ERROR "Only one of ENABLE_DEBUG, ENABLE_O1, ENABLE_O2, or ENABLE_O3 can be enabled at a time.")
endif()


# ===========================
# APPLY COMPILER OPTIONS BASED ON CONFIGURATION
# ===========================

if (ENABLE_DEBUG)
    add_compile_options(-O0 -g)
elseif(ENABLE_O0)
    add_compile_options(-O0 -march=native)
elseif (ENABLE_O1)
    add_compile_options(-O1 -march=native)
elseif (ENABLE_O2)
    add_compile_options(-O2 -march=native)
else()
    add_compile_options(-O3 -march=native) # Default optimized build
endif()

# Apply function inlining disable
if (ENABLE_NO_INLINE)
    add_compile_options(-fno-inline)
endif()

# Allow profiling 
if (ENABLE_PROFILING)
    add_compile_options(-pg)
endif()

# Enable verbose error messages for C++ Concepts (if using GCC)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND ENABLE_CONCEPT_ERROR_DETAIL)
    add_compile_options(-fconcepts-diagnostics-depth=5)
endif()

message(STATUS "Building with compile options: ${CMAKE_CXX_FLAGS}")


# ===========================
# Fetch Dependencies
# ===========================

include(FetchContent)
include(CMakeFindDependencyMacro)

cmake_policy(SET CMP0135 NEW)

FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/ec25eea8f8237cf86c30703f59747e42f34b6f75.zip)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# ===========================
# DEFINE SRC FILES
# ===========================

# Define all source files in a variable
# NOTE: If project gets any bigger, recommend using
#       multiple libraries.
set(SRC_FILES
    src/cli_utils.cpp
    src/benchmark_utils.cpp
    src/runtime_polymorphism.cpp
    src/crtp_polymorphism.cpp
    src/concepts_polymorphism.cpp
    src/polymorphism_tests.cpp
    src/test_runner.cpp
)

# ===========================
# BUILD TESTS
# ===========================


# Define test output directory
set(TEST_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test)

add_executable(test_cli_utils test/core/test_cli_utils.cpp ${SRC_FILES})
target_include_directories(test_cli_utils PRIVATE include)
target_link_libraries(
    test_cli_utils PRIVATE
    GTest::gtest_main
    )

# Ensure test_cli_utils is placed in ./build/bin/test/
set_target_properties(test_cli_utils PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})

add_executable(test_benchmark_utils test/core/test_benchmark_utils.cpp ${SRC_FILES})
target_include_directories(test_benchmark_utils PRIVATE include)
target_link_libraries(test_benchmark_utils PRIVATE GTest::gtest_main)

# Ensure test_benchmark_utils is placed in ./build/bin/test/
set_target_properties(test_benchmark_utils PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})


# ===========================
# BUILD TARGET
# ===========================

# Use the SRC_FILES variable in add_executable
add_executable(benchmark src/main.cpp ${SRC_FILES})

# ===========================
# INCLUDE DIRECTORIES
# ===========================

target_include_directories(benchmark PRIVATE include)

# ===========================
# ADD DEFINITIONS
# ===========================

target_compile_definitions(benchmark PRIVATE COMPILER_FLAGS="${CMAKE_CXX_FLAGS}")
target_compile_definitions(test_cli_utils PRIVATE COMPILER_FLAGS="${CMAKE_CXX_FLAGS}")
target_compile_definitions(test_benchmark_utils PRIVATE COMPILER_FLAGS="${CMAKE_CXX_FLAGS}")



