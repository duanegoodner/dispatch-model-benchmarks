
#include "test_runner.hpp"
#include "benchmark_utils.hpp"
#include "polymorphism_tests.hpp"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

// Use the macro defined in CMakeLists.txt
#ifndef COMPILER_FLAGS
#define COMPILER_FLAGS "Unknown"
#endif

namespace test_runner {

// Retrieve the test case map
const std::unordered_map<std::string, std::unordered_map<std::string, TestCase>>
    &GetTestCaseMap() {
  // TODO: Need to edit this if want to add/remove polymorphism type or compute
  // function. In future, consider building from a file for easier extension.
  static const std::
      unordered_map<std::string, std::unordered_map<std::string, TestCase>>
          test_case_map = {
              {"runtime",
               {{"fma",
                 {"polymorphism_tests::TestRuntimeFMA",
                  polymorphism_tests::TestRuntimeFMA}},
                {"expensive",
                 {"polymorphism_tests::TestRuntimeExpensive",
                  polymorphism_tests::TestRuntimeExpensive}}}},
              {"crtp",
               {{"fma",
                 {"polymorphism_tests::TestCRTPFMA",
                  polymorphism_tests::TestCRTPFMA}},
                {"expensive",
                 {"polymorphism_tests::TestCRTPExpensive",
                  polymorphism_tests::TestCRTPExpensive}}}},
              {"concepts",
               {{"fma",
                 {"polymorphism_tests::TestConceptsFMA",
                  polymorphism_tests::TestConceptsFMA}},
                {"expensive",
                 {"polymorphism_tests::TestConceptsExpensive",
                  polymorphism_tests::TestConceptsExpensive}}}}};

  return test_case_map;
}

// Retrieve a single test case for a particular combination of
// polymorphism type + computation
const TestCase &GetSingleTestCase(
    const std::string &polymorphism_category,
    const std::string &computation
) {
  const auto &test_case_map = GetTestCaseMap();

  auto pc_it = test_case_map.find(polymorphism_category);
  if (pc_it == test_case_map.end()) {
    throw std::invalid_argument("Invalid namespace: " + polymorphism_category);
  }

  auto comp_it = pc_it->second.find(computation);
  if (comp_it == pc_it->second.end()) {
    throw std::invalid_argument("Invalid computation: " + computation);
  }

  return comp_it->second;
}

// Run a single test
std::chrono::duration<double> RunSingleTest(
    const std::string &polymorphism_category,
    const std::string &computation_label,
    size_t iterations,
    bool write_to_file
) {
  const auto &test_case =
      GetSingleTestCase(polymorphism_category, computation_label);

  auto elapsed_time = RunTestCase(test_case, iterations);

  // If requested, write results to a file
  if (write_to_file) {
    std::string output_dir = "data/single_test_results/";
    WriteSingleTestResultToFile(
        output_dir,
        iterations,
        polymorphism_category,
        computation_label,
        elapsed_time
    );
  }
  return elapsed_time;
}

// Run all tests
void RunAndSaveAllTests(size_t iterations) {

  // Define output directory
  std::string output_dir = "data/run_all_tests_results/";

  // Generate timestamp-based filename
  auto filepath = GenerateTimestampBasedFile(output_dir);

  // Open file for writing
  std::ofstream outfile(filepath);
  ValidateOutfileStream(outfile, filepath);
  WriteCompileFlagsInfo(outfile);
  WriteNumberOfIterations(iterations, outfile);
  WriteMarkdownTableHeader(outfile);

  const auto &test_case_map = GetTestCaseMap();

  // For each inner entry in nested map, run test and write data
  for (const auto &[polymorphism_type, inner_map] : test_case_map) {
    for (const auto &[compute_function, test_case] : inner_map) {
      auto elapsed_time =
          RunSingleTest(polymorphism_type, compute_function, iterations, false);
      WriteMarkdownTableRow(
          outfile,
          polymorphism_type,
          compute_function,
          elapsed_time
      );
    }
  }

  std::cout << "Test results saved to: " << filepath << std::endl << std::endl;
}

void RunAllTestsWithoutSaving(size_t iterations) {
  // For each inner entry in nested map, run test
  const auto &test_case_map = GetTestCaseMap();
  for (const auto &[polymorphism_type, inner_map] : test_case_map) {
    for (const auto &[compute_function, test_case] : inner_map) {
      auto elapsed_time =
          RunSingleTest(polymorphism_type, compute_function, iterations, false);
    }
  }
}

void RunAllTests(size_t iterations, bool save_execution_times) {
  if (save_execution_times) {
    RunAndSaveAllTests(iterations);
  } else {
    RunAllTestsWithoutSaving(iterations);
  }
}
} // namespace test_runner
