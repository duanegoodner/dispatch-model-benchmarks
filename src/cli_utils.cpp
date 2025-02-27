#include "cli_utils.hpp"
#include <iostream>
#include <limits>
#include <sstream>

// Prints usage information for the command-line tool
void PrintUsage(const char *program_name) {
  std::cerr
      << "Usage: " << program_name
      << " [polymorphism_category] [computation] [-n iterations]\n"
      << " - No arguments: Runs all tests with the default iteration count.\n"
      << " - With '-n iterations': Runs all tests with a custom iteration "
         "count.\n"
      << " - With two arguments: Runs a specific test with the default "
         "iteration count.\n"
      << " - With three arguments plus '-n iterations': Runs a specific test "
         "with a custom iteration count.\n"
      << "   Valid values for polymorphism_category: ";

  const auto &test_case_map = test_runner::GetTestCaseMap();
  for (const auto &category : test_case_map) {
    std::cerr << category.first << " ";
  }

  std::cerr << "\n   Valid values for computation: ";
  if (!test_case_map.empty()) {
    for (const auto &computation : test_case_map.begin()->second) {
      std::cerr << computation.first << " ";
    }
  }
  std::cerr << "\n   -n [iterations]: A positive integer specifying the number "
               "of iterations.\n";
  std::cerr << std::endl;
}

// Validates whether the given polymorphism category exists
bool IsValidPolymorphismCategory(const std::string &category) {
  const auto &test_case_map = test_runner::GetTestCaseMap();
  return test_case_map.find(category) != test_case_map.end();
}

// Validates whether the given computation exists for the specified polymorphism
// category
bool IsValidComputation(
    const std::string &category,
    const std::string &computation
) {
  const auto &test_case_map = test_runner::GetTestCaseMap();
  auto category_it = test_case_map.find(category);
  if (category_it == test_case_map.end()) {
    return false;
  }
  return category_it->second.find(computation) != category_it->second.end();
}

std::optional<size_t> ParseIterationCount(
    int argc,
    char **argv,
    int &arg_index,
    int &remaining_argc
) {
  for (int i = 1; i < argc - 1;
       ++i) { // Look for "-n" anywhere except the last argument
    if (std::string_view(argv[i]) == "-n") {
      std::istringstream iss(argv[i + 1]);
      size_t iterations;
      if (!(iss >> iterations) || iterations == 0 || !iss.eof()) {
        return std::nullopt; // Invalid input
      }

      // Shift remaining arguments forward
      for (int j = i; j < argc - 2; ++j) {
        argv[j] = argv[j + 2];
      }

      // Reduce argument count
      remaining_argc -= 2;

      return iterations;
    }
  }
  return std::nullopt;
}
