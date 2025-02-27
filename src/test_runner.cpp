
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "benchmark_utils.hpp"
#include "test_runner.hpp"
#include "polymorphism_tests.hpp"

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
void RunSingleTest(
  const std::string &polymorphism_category,
  const std::string &computation_label,
  size_t iterations,
  bool write_to_file
) {
const auto &test_case =
    GetSingleTestCase(polymorphism_category, computation_label);
std::cout << "Running: " << test_case.name << std::endl;

auto start = std::chrono::high_resolution_clock::now();
test_case.function(iterations);
auto end = std::chrono::high_resolution_clock::now();
std::chrono::duration<double> duration = end - start;

// If requested, write results to a file
if (write_to_file) {
  std::string output_dir = "data/single_test_results/";
  auto filepath = GenerateTimestampBasedFile(output_dir);
  
  // std::filesystem::create_directories(output_dir);

  // // Generate timestamp-based filename
  // auto now = std::chrono::system_clock::now();
  // auto now_time = std::chrono::system_clock::to_time_t(now);
  // auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
  //                   now.time_since_epoch()) % 1000;

  // std::ostringstream filename;
  // filename << output_dir
  //          << std::put_time(std::localtime(&now_time), "%Y-%m-%d-%H-%M-%S")
  //          << "-" << now_ms.count() << ".txt";

  // std::string filepath = filename.str();

  std::ofstream outfile(filepath);
  if (!outfile) {
    std::cerr << "Error: Unable to open file for writing: " << filepath << std::endl;
    return;
  }

  // Write test details
  outfile << "Compiler Flags: " << COMPILER_FLAGS << "\n\n";
  outfile << "| Polymorphism Type | Compute Function | Time (seconds) |\n";
  outfile << "|-------------------|-----------------|---------------|\n";
  outfile << "| " << polymorphism_category << " | " << computation_label << " | " << duration.count() << " |\n";

  std::cout << "Single test result saved to: " << filepath << std::endl;
}
}


// Run all tests
void RunAllTests(size_t iterations) {
  static const std::vector<std::pair<std::string, std::string>> test_pairs = {
      {"runtime", "fma"},
      {"runtime", "expensive"},
      {"crtp", "fma"},
      {"crtp", "expensive"},
      {"concepts", "fma"},
      {"concepts", "expensive"}};

  // Define output directory
  std::string output_dir = "data/run_all_tests_results/";

  // Generate timestamp-based filename
  auto filepath = GenerateTimestampBasedFile(output_dir); 
  // auto now = std::chrono::system_clock::now();
  // auto now_time = std::chrono::system_clock::to_time_t(now);
  // auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
  //                   now.time_since_epoch()
  //               ) %
  //               1000;

  // std::ostringstream filename;
  // filename << output_dir
  //          << std::put_time(std::localtime(&now_time), "%Y-%m-%d-%H-%M-%S")
  //          << "-" << now_ms.count() << ".txt";

  // std::string filepath = filename.str();

  // Ensure output directory exists
  // std::filesystem::create_directories(output_dir);

  // Open file for writing
  std::ofstream outfile(filepath);
  if (!outfile) {
    std::cerr << "Error: Unable to open file for writing: " << filepath
              << std::endl;
    return;
  }

  // Write compiler flags at the top of the file
  outfile << "Compiler Flags: " << COMPILER_FLAGS << "\n\n";

  // Write Markdown table header
  outfile << "| Polymorphism Type | Compute Function | Time (seconds) |\n";
  outfile << "|-------------------|-----------------|---------------|\n";

  // Run each test and collect results
  for (const auto &[category, label] : test_pairs) {
    auto start = std::chrono::high_resolution_clock::now();

    RunSingleTest(category, label, iterations, false);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Write result in Markdown format
    outfile << "| " << category << " | " << label << " | " << duration.count()
            << " |\n";
  }

  std::cout << "Test results saved to: " << filepath << std::endl;
}
} // namespace test_runner
