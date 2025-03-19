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
option(ENABLE_O0 "No compiler optimization -O0" OFF)
option(ENABLE_O1 "Compiler optimization -O1" OFF)
option(ENABLE_O2 "Compiler optimization -O2" OFF)
option(ENABLE_O3 "Compiler optimization -O3" OFF)
option(ENABLE_DEBUG "Enable Debugging Symbols -g" OFF)
option(ENABLE_NO_INLINE "Disable Function Inlining -fno-inline" OFF)
option(ENABLE_PROFILING "Enable Profiling -p (for gprof)" OFF)
option(ENABLE_CONCEPT_ERROR_DETAIL "Enable Verbose Compiler Errors for Concepts" OFF)

# ===========================
# HANDLE RESET_DEFAULTS OPTION
# ===========================

if (RESET_DEFAULTS)
    message(STATUS "Resetting all options to default settings (turning them OFF)...")
    set(ENABLE_O0 OFF CACHE BOOL "Disable -O0" FORCE)
    set(ENABLE_O1 OFF CACHE BOOL "Disable -O1" FORCE)
    set(ENABLE_O2 OFF CACHE BOOL "Disable -O2" FORCE)
    set(ENABLE_O3 OFF CACHE BOOL "Disable -O3" FORCE)
    set(ENABLE_DEBUG OFF CACHE BOOL "Disable Debugging Symbols (Reset to Default)" FORCE)
    set(ENABLE_NO_INLINE OFF CACHE BOOL "Allow Function Inlining (Reset to Default)" FORCE)
    set(ENABLE_PROFILING OFF CACHE BOOL "Disable Profiling (Reset to Default)" FORCE)
    set(ENABLE_CONCEPT_ERROR_DETAIL OFF CACHE BOOL "Enable Verbose Compiler Errors for Concepts (Default)" FORCE)
endif()

# ===========================
# APPLY COMPILER OPTIONS BASED ON CONFIGURATION
# ===========================

# Create an empty variable to track flags
set(MY_COMPILE_FLAGS "")

if (ENABLE_O0)
    add_compile_options(-O0 -march=native)
    set(MY_COMPILE_FLAGS "${MY_COMPILE_FLAGS} -O0 -march=native")
elseif (ENABLE_O1)
    add_compile_options(-O1 -march=native)
    set(MY_COMPILE_FLAGS "${MY_COMPILE_FLAGS} -O1 -march=native")
elseif (ENABLE_O2)
    add_compile_options(-O2 -march=native)
    set(MY_COMPILE_FLAGS "${MY_COMPILE_FLAGS} -O2 -march=native")
else()
    add_compile_options(-O3 -march=native) # Default optimized build
    set(MY_COMPILE_FLAGS "${MY_COMPILE_FLAGS} -O3 -march=native")
endif()

if (ENABLE_DEBUG)
    add_compile_options(-g)
    set(MY_COMPILE_FLAGS "${MY_COMPILE_FLAGS} -g")
endif()

# Apply function inlining disable
if (ENABLE_NO_INLINE)
    add_compile_options(-fno-inline)
    set(MY_COMPILE_FLAGS "${MY_COMPILE_FLAGS} -fno-inline")
endif()

# Enable verbose error messages for C++ Concepts (if using GCC)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND ENABLE_CONCEPT_ERROR_DETAIL)
    add_compile_options(-fconcepts-diagnostics-depth=5)
    set(MY_COMPILE_FLAGS "${MY_COMPILE_FLAGS} -fconcepts-diagnostics-depth=5")
endif()

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

get_target_property(BENCHMARK_COMPILER_FLAGS benchmark COMPILE_OPTIONS)
message(STATUS "Compile options for benchmark: ${BENCHMARK_COMPILER_FLAGS}")
message(STATUS "MY_COMPILE_FLAGS = ${MY_COMPILE_FLAGS}")

target_compile_definitions(benchmark PRIVATE COMPILER_FLAGS="${MY_COMPILE_FLAGS}")
target_compile_definitions(test_cli_utils PRIVATE COMPILER_FLAGS="${MY_COMPILE_FLAGS}")
target_compile_definitions(test_benchmark_utils PRIVATE COMPILER_FLAGS="${MY_COMPILE_FLAGS}")