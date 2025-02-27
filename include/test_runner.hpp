#pragma once

#include "benchmark_utils.hpp"
#include <unordered_map>
#include <vector>

namespace test_runner {

struct TestCase {
  std::string name;
  void (*function)(size_t);
};

const std::unordered_map<std::string, std::unordered_map<std::string, TestCase>>
    &GetTestCaseMap();
const TestCase &GetSingleTestCase(
    const std::string &polymorphism_category,
    const std::string &computation_label
);
void RunSingleTest(
    const std::string &polymorphism_category,
    const std::string &computation_label,
    size_t iterations
);
void RunAllTests(size_t iterations);

} // namespace test_runner
