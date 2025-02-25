cmake_minimum_required(VERSION 3.20)
message(STATUS "Using CMake version ${CMAKE_VERSION}")

project(PolymorphismBenchmark LANGUAGES CXX VERSION 0.1.0)
message(STATUS "Project name: ${PROJECT_NAME}")
message(STATUS "Project version: ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable verbose Concepts error messages (optional)
option(ENABLE_CONCEPT_ERROR_DETAIL "Verbose compiler errors for Concepts" ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND ENABLE_CONCEPT_ERROR_DETAIL)
    add_compile_options(-fconcepts-diagnostics-depth=5)
endif()

include_directories(include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add source files
add_executable(benchmark src/main.cpp src/utils.cpp)