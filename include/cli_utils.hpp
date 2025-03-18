#pragma once

#include "test_runner.hpp"
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
ParseIterationCount(int argc, char **argv, int &remaining_argc);

// Parses the "-s" flag to enable saving execution time data
bool ParseSaveExecutionTimesFlag(int argc, char **argv, int &remaining_argc);

// Handles command-line arguments and runs tests
int RunFromCLI(int argc, char **argv);

// Helper functions to clean up RunFromCLI
bool HandleHelpOption(int argc, char **argv);
std::optional<size_t> ParseAndValidateArguments(
    int argc,
    char **argv,
    int &remaining_argc
);
int RunAppropriateTests(
    int remaining_argc,
    char **argv,
    size_t iterations,
    bool save_execution_times
);
