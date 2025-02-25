#include <iostream>
#include <vector>
#include "concepts_polymorphism.hpp"
#include "crtp_polymorphism.hpp"
#include "plain_templates.hpp"
#include "runtime_polymorphism.hpp"
#include "test_runner.hpp"

int main() {
    constexpr size_t kNumIterations = 1000000000;

    std::vector<std::string> testLabels = {
        "Minimal Work Function:",
        "FMA Computation:",
        "Simple Computation:",
        "Medium Computation:",
        "Expensive Computation:"
    };

    std::cout << "\n--- Testing Runtime Polymorphism ---\n";
    runtime_polymorphism::RunTests(kNumIterations, testLabels);

    std::cout << "\n--- Testing Plain Templates ---\n";
    plain_templates::RunTests(kNumIterations, testLabels);

    std::cout << "\n--- Testing CRTP Polymorphism ---\n";
    crtp_polymorphism::RunTests(kNumIterations, testLabels);

    std::cout << "\n--- Testing C++20 Concepts Polymorphism ---\n";
    concepts_polymorphism::RunTests(kNumIterations, testLabels);

    return 0;
}
