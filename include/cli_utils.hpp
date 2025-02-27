#pragma once

#include <string>
#include <unordered_map>
#include "benchmark_utils.hpp"

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
