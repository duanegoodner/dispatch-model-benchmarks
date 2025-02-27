#pragma once

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

struct TestCase {
  std::string name;
  void (*function)(size_t);
};

// External declaration for preventing compiler optimizations
extern volatile double prevent_optimization;

// Utility function to print elapsed time
void PrintTime(const std::string &label, std::chrono::duration<double> elapsed);

std::chrono::duration<double> RunTestCase(
    const TestCase &test_case,
    size_t iterations
);

std::string GenerateTimestampBasedFile(std::string output_dir);

int ValidateOutfileStream(std::ofstream &outfile, const std::string &filepath);

void WriteCompileFlagsInfo(std::ofstream &outfile);

void WriteNumberOfIterations(size_t num_iterations, std::ofstream &outfile);

void WriteMarkdownTableHeader(std::ofstream &outfile);

void WriteMarkdownTableRow(
    std::ofstream &outfile,
    const std::string &polymorphism_category,
    const std::string &computation_label,
    std::chrono::duration<double> elapsed_time
);

void WriteSingleTestResultToFile(
    const std::string &output_dir,
    size_t iterations,
    const std::string &polymorphism_category,
    const std::string &computation_label,
    std::chrono::duration<double> elapsed_time
);

// Generalized benchmarking function
template <typename Callable>
std::chrono::duration<double> RunBenchmark(
    const std::string &label,
    size_t n,
    Callable &&compute_func
) {
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
