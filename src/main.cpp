#include <iostream>
#include "test_runner.hpp"

int main() {
    constexpr size_t kNumIterations = 1000000000;

    std::cout << "\n--- Testing Runtime Polymorphism ---\n";
    runtime_polymorphism::RunTests(kNumIterations);

    std::cout << "\n--- Testing CRTP Polymorphism ---\n";
    crtp_polymorphism::RunTests(kNumIterations);

    std::cout << "\n--- Testing C++20 Concepts Polymorphism ---\n";
    concepts_polymorphism::RunTests(kNumIterations);

    return 0;
}

