#pragma once

#include "benchmark_utils.hpp"
#include "concepts_polymorphism.hpp"
#include "crtp_polymorphism.hpp"
#include "runtime_polymorphism.hpp"
#include <functional>
#include <unordered_map>
#include <vector>

namespace runtime_polymorphism {
void TestFMA(size_t iterations);
void TestExpensive(size_t iterations);
void RunTests(size_t iterations);
} // namespace runtime_polymorphism

namespace crtp_polymorphism {
void TestFMA(size_t iterations);
void TestExpensive(size_t iterations);
void RunTests(size_t iterations);
} // namespace crtp_polymorphism

namespace concepts_polymorphism {
void TestFMA(size_t iterations);
void TestExpensive(size_t iterations);
void RunTests(size_t iterations);
} // namespace concepts_polymorphism

// Define a struct to hold test case information
struct TestCase {
  std::string name;
  void (*function)(size_t); // Function pointer to the test function
};

void RunAllTests(size_t iterations);

const TestCase &GetSingleTestCase(
    const std::string &polymorphism_category,
    const std::string &computation_label
);
void RunSingleTest(
    const std::string &polymorphism_category,
    const std::string &computation_label,
    size_t iterations
);