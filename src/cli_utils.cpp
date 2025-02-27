#include "cli_utils.hpp"
#include <iostream>

// Prints usage information for the command-line tool
void PrintUsage(const char* program_name) {
    std::cerr << "Usage: " << program_name
              << " [polymorphism_category] [computation]\n"
              << " - No arguments: Runs all tests.\n"
              << " - With arguments: Runs a specific test.\n"
              << "   Valid values for polymorphism_category: ";

    const auto& test_case_map = test_runner::GetTestCaseMap();
    for (const auto& category : test_case_map) {
        std::cerr << category.first << " ";
    }

    std::cerr << "\n   Valid values for computation: ";
    if (!test_case_map.empty()) {
        for (const auto& computation : test_case_map.begin()->second) {
            std::cerr << computation.first << " ";
        }
    }
    std::cerr << std::endl;
}

// Validates whether the given polymorphism category exists
bool IsValidPolymorphismCategory(const std::string& category) {
    const auto& test_case_map = test_runner::GetTestCaseMap();
    return test_case_map.find(category) != test_case_map.end();
}

// Validates whether the given computation exists for the specified polymorphism category
bool IsValidComputation(const std::string& category, const std::string& computation) {
    const auto& test_case_map = test_runner::GetTestCaseMap();
    auto category_it = test_case_map.find(category);
    if (category_it == test_case_map.end()) {
        return false;
    }
    return category_it->second.find(computation) != category_it->second.end();
}
