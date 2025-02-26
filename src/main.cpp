#include <iostream>
#include "test_runner.hpp"

int main() {
    constexpr size_t kNumIterations = 1000000000;

    RunAllTests(kNumIterations);

    return 0;
}

