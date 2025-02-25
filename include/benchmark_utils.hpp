#pragma once

#include <chrono>
#include <string>
#include "utils.hpp"

// Generalized benchmarking function
template <typename Callable>
void RunBenchmark(const std::string& label, size_t n, Callable&& computeFunc) {
    auto start = std::chrono::high_resolution_clock::now();
    double sum = 0.0;
    for (size_t i = 0; i < n; ++i) {
        sum += computeFunc(2.0);
    }
    prevent_optimization = sum;
    auto end = std::chrono::high_resolution_clock::now();
    PrintTime(label, end - start);
}
