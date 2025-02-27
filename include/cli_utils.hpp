#pragma once

#include "benchmark_utils.hpp"
#include <optional>
#include <string>
#include <unordered_map>

// Forward declaration
namespace test_runner {
const std::unordered_map<std::string, std::unordered_map<std::string, TestCase>>
    &GetTestCaseMap();
}

// Prints usage information
void PrintUsage(const char *program_name);

// Validates the polymorphism category
bool IsValidPolymorphismCategory(const std::string &category);

// Validates the computation type for a given polymorphism category
bool IsValidComputation(
    const std::string &category,
    const std::string &computation
);

// Parses an optional "-n [iterations]" argument
std::optional<size_t>
ParseIterationCount(int argc, char **argv, int &arg_index, int &remaining_argc);
