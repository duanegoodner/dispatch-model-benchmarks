#include "cli_utils.hpp"
#include "test_runner.hpp"
#include <cstdlib>
#include <iostream>
#include <string_view>

// Default number of iterations
constexpr size_t kDefaultNumIterations = 1'000'000'000;

// Prints usage information for the command-line tool
void PrintUsage(const char *program_name) {
  std::cerr
      << "\nUsage: " << program_name
      << " [polymorphism_category] [computation] [-n iterations]\n"
      << " - No arguments: Runs all tests with the default iteration count.\n"
      << " - With '-n iterations': Runs all tests with a custom iteration "
         "count.\n"
      << " - With two arguments: Runs a specific test with the default "
         "iteration count.\n"
      << " - With three arguments plus '-n iterations': Runs a specific test "
         "with a custom iteration count.\n\n"
      << "Valid options:\n"
      << " ------------------------\n";

  // Retrieve valid test cases
  const auto &test_case_map = test_runner::GetTestCaseMap();

  // Format valid polymorphism categories
  std::cerr << " Polymorphism Categories:\n";
  std::cerr << " ------------------------\n";
  for (const auto &category : test_case_map) {
    std::cerr << "  - " << category.first << "\n";
  }

  // Format valid computation functions
  std::cerr << "\n Compute Functions:\n";
  std::cerr << " ------------------\n";
  if (!test_case_map.empty()) {
    for (const auto &computation : test_case_map.begin()->second) {
      std::cerr << "  - " << computation.first << "\n";
    }
  }

  std::cerr << "\nOther Options:\n"
            << "  --help              Show this help message\n"
            << "  -n [iterations]     Specify a custom iteration count\n"
            << std::endl;
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

// Parses the optional "-n [iterations]" argument, removes it from args, and
// returns the parsed value
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

// Checks for the "--help" option and prints usage if needed
bool HandleHelpOption(int argc, char **argv) {
  if (argc == 2 && std::string_view(argv[1]) == "--help") {
    PrintUsage(argv[0]);
    return true; // Indicate that help was shown
  }
  return false;
}

// Parses and validates arguments, returning the iteration count if provided
std::optional<size_t> ParseAndValidateArguments(
    int argc,
    char **argv,
    int &remaining_argc
) {
  int arg_index = 1;
  auto parsed_iterations =
      ParseIterationCount(argc, argv, arg_index, remaining_argc);
  return parsed_iterations.value_or(kDefaultNumIterations
  ); // Default if not provided
}

// Determines whether to run all tests or a single test
int RunAppropriateTests(int remaining_argc, char **argv, size_t iterations) {
  if (remaining_argc == 1) {
    // No arguments left → Run all tests
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

// Handles command-line arguments and runs the appropriate test(s)
int RunFromCLI(int argc, char **argv) {
  if (HandleHelpOption(argc, argv)) {
    return EXIT_SUCCESS; // Stop execution if help was printed
  }

  int remaining_argc = argc;
  std::optional<size_t> maybe_iterations =
      ParseAndValidateArguments(argc, argv, remaining_argc);
  size_t iterations = maybe_iterations.value_or(kDefaultNumIterations);

  //   Print the iteration count for logging in the shell script
//   std::cout << "Iteration Count: " << iterations << std::endl;

  return RunAppropriateTests(remaining_argc, argv, iterations);
}
