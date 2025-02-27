#pragma once

#include <chrono>
#include <iostream>
#include <string>

// External declaration for preventing compiler optimizations
extern volatile double prevent_optimization;

// Utility function to print elapsed time
void PrintTime(const std::string &label, std::chrono::duration<double> elapsed);

// Generalized benchmarking function
template <typename Callable>
std::chrono::duration<double> RunBenchmark(const std::string &label, size_t n, Callable &&compute_func) {
  auto start = std::chrono::high_resolution_clock::now();
  double sum = 0.0;
  for (size_t i = 0; i < n; ++i) {
    sum += compute_func(2.0);
  }
  prevent_optimization = sum; // don't let compiler optimize out our test loop
  auto end = std::chrono::high_resolution_clock::now();
  auto elapsed = end - start;
  PrintTime(label, elapsed);
  
  return elapsed;
}