#include "test_runner.hpp"
#include "cli_utils.hpp"
#include <cstdlib>
#include <iostream>
#include <string_view>

// Number of iterations for each test
constexpr size_t kNumIterations = 1'000'000'000;

int main(int argc, char **argv) {
    // Handle --help option
    if (argc == 2 && std::string_view(argv[1]) == "--help") {
        PrintUsage(argv[0]);
        return EXIT_SUCCESS;
    }

    if (argc == 1) {
        // No arguments: Run all tests
        test_runner::RunAllTests(kNumIterations);
    } else if (argc == 3) {
        std::string polymorphism_category = argv[1];
        std::string computation = argv[2];

        // Validate input arguments
        if (!IsValidPolymorphismCategory(polymorphism_category)) {
            std::cerr << "Error: Invalid polymorphism category '" << polymorphism_category << "'\n";
            PrintUsage(argv[0]);
            return EXIT_FAILURE;
        }

        if (!IsValidComputation(polymorphism_category, computation)) {
            std::cerr << "Error: Invalid computation type '" << computation << "'\n";
            PrintUsage(argv[0]);
            return EXIT_FAILURE;
        }

        test_runner::RunSingleTest(polymorphism_category, computation, kNumIterations, true);
    } else {
        // Invalid number of arguments
        PrintUsage(argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
