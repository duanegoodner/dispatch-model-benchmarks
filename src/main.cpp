#include "test_runner.hpp"
#include <iostream>
#include <cstdlib>  // For EXIT_SUCCESS and EXIT_FAILURE
#include <string_view>

constexpr size_t kNumIterations = 1'000'000'000;  // More readable with digit separators

void PrintUsage(const char* program_name) {
    std::cerr << "Usage: " << program_name << " [polymorphism_category] [computation]\n"
              << " - No arguments: Runs all tests.\n"
              << " - With arguments: Runs a specific test.\n"
              << "   Valid categories: runtime, crtp, concepts\n"
              << "   Valid computations: fma, expensive\n";
}

int main(int argc, char** argv) {
    if (argc == 1) {
        RunAllTests(kNumIterations);
    } else if (argc == 3) {
        std::string_view polymorphism_category = argv[1];
        std::string_view computation = argv[2];
        RunSingleTest(std::string(polymorphism_category), std::string(computation), kNumIterations);
    } else {
        PrintUsage(argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
