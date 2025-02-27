#include "cli_utils.hpp"
#include "test_runner.hpp"
#include <cstdlib>
#include <iostream>
#include <string_view>

// Default number of iterations
constexpr size_t kDefaultNumIterations = 1'000'000'000;

int main(int argc, char **argv) {
  // Handle --help option
  if (argc == 2 && std::string_view(argv[1]) == "--help") {
    PrintUsage(argv[0]);
    return EXIT_SUCCESS;
  }

  int remaining_argc = argc; // Track remaining valid arguments
  size_t iterations = kDefaultNumIterations;

  // Extract iteration count (if provided)
  int arg_index = 1;
  auto parsed_iterations =
      ParseIterationCount(argc, argv, arg_index, remaining_argc);
  if (parsed_iterations) {
    iterations = *parsed_iterations;
  }

  if (remaining_argc == 1) {
    // No other arguments → Run all tests
    test_runner::RunAllTests(iterations);
  } else if (remaining_argc == 3) {
    std::string polymorphism_category = argv[1];
    std::string computation = argv[2];

    // Validate input arguments
    if (!IsValidPolymorphismCategory(polymorphism_category)) {
      std::cerr << "Error: Invalid polymorphism category '"
                << polymorphism_category << "'\n";
      PrintUsage(argv[0]);
      return EXIT_FAILURE;
    }

    if (!IsValidComputation(polymorphism_category, computation)) {
      std::cerr << "Error: Invalid computation type '" << computation << "'\n";
      PrintUsage(argv[0]);
      return EXIT_FAILURE;
    }

    test_runner::RunSingleTest(
        polymorphism_category,
        computation,
        iterations,
        true
    );
  } else {
    // Invalid number of arguments
    PrintUsage(argv[0]);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
