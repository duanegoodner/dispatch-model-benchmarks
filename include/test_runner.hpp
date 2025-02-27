// Declarations needed for running tests.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "benchmark_utils.hpp"


namespace test_runner {

const std::unordered_map<std::string, std::unordered_map<std::string, TestCase>>
    &GetTestCaseMap();
const TestCase &GetSingleTestCase(
    const std::string &polymorphism_category,
    const std::string &computation_label
);
std::chrono::duration<double> RunSingleTest(
    const std::string &polymorphism_category,
    const std::string &computation_label,
    size_t iterations,
    bool write_to_file
);
void RunAllTests(size_t iterations);

} // namespace test_runner
