// Allows testing of either all possible pairs of
// polymorphism-type + computation-function a specific pair.

#include "test_runner.hpp"
#include <cstdlib>
#include <iostream>
#include <string_view>

// number of iterations for test of a
// polymorphism type / compute function pair
constexpr size_t kNumIterations = 1'000'000'000;

void PrintUsage(const char *program_name) {
  std::cerr << "Usage: " << program_name
            << " [polymorphism_category] [computation]\n"
            << " - No arguments: Runs all tests.\n"
            << " - With arguments: Runs a specific test.\n"
            << "   Valid categories: ";

  const auto &testCaseMap = test_runner::GetTestCaseMap();
  for (const auto &category : testCaseMap) {
    std::cerr << category.first << " ";
  }

  std::cerr << "\n   Valid computations: ";
  if (!testCaseMap.empty()) {
    for (const auto &computation : testCaseMap.begin()->second) {
      std::cerr << computation.first << " ";
    }
  }
  std::cerr << std::endl;
}

int main(int argc, char **argv) {
  if (argc == 1) {
    //  if no args provide, then run all possible tests
    test_runner::RunAllTests(kNumIterations);
  } else if (argc == 3) {
    std::string_view polymorphism_category = argv[1];
    std::string_view computation = argv[2];

    // keys in this nested unordered_map determine allowed arg values
    const auto &testCaseMap = test_runner::GetTestCaseMap();

    // confirm polymorphism_category exists
    if (testCaseMap.find(std::string(polymorphism_category)) ==
        testCaseMap.end()) {
      std::cerr << "Error: Invalid polymorphism category '"
                << polymorphism_category << "'\n";
      PrintUsage(argv[0]);
      return EXIT_FAILURE;
    }

    // confirm computation function name exists
    if (testCaseMap.at(std::string(polymorphism_category))
            .find(std::string(computation)) ==
        testCaseMap.at(std::string(polymorphism_category)).end()) {
      std::cerr << "Error: Invalid computation type '" << computation << "'\n";
      PrintUsage(argv[0]);
      return EXIT_FAILURE;
    }

    test_runner::RunSingleTest(
        std::string(polymorphism_category),
        std::string(computation),
        kNumIterations
    );
  } else {
    // invalid number of args
    PrintUsage(argv[0]);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
