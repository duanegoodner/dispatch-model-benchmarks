#include "test_runner.hpp"
#include "polymorphism_tests.hpp"
#include <iostream>
#include <stdexcept>

namespace test_runner {

// Retrieve the test case map
const std::unordered_map<std::string, std::unordered_map<std::string, TestCase>>
    &GetTestCaseMap() {
  static const std::
      unordered_map<std::string, std::unordered_map<std::string, TestCase>>
          testCaseMap = {
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

  return testCaseMap;
}

// Retrieve a single test case
const TestCase &GetSingleTestCase(
    const std::string &ns,
    const std::string &computation
) {
  const auto &testCaseMap = GetTestCaseMap();

  auto nsIt = testCaseMap.find(ns);
  if (nsIt == testCaseMap.end()) {
    throw std::invalid_argument("Invalid namespace: " + ns);
  }

  auto compIt = nsIt->second.find(computation);
  if (compIt == nsIt->second.end()) {
    throw std::invalid_argument("Invalid computation: " + computation);
  }

  return compIt->second;
}

// Run a single test
void RunSingleTest(
    const std::string &polymorphism_category,
    const std::string &computation_label,
    size_t iterations
) {
  const auto &test_case =
      GetSingleTestCase(polymorphism_category, computation_label);
  std::cout << "Running: " << test_case.name << std::endl;
  test_case.function(iterations);
}

// Run all tests
void RunAllTests(size_t iterations) {
  static const std::vector<std::pair<std::string, std::string>> testPairs = {
      {"runtime", "fma"},
      {"runtime", "expensive"},
      {"crtp", "fma"},
      {"crtp", "expensive"},
      {"concepts", "fma"},
      {"concepts", "expensive"}};

  for (const auto &[category, label] : testPairs) {
    RunSingleTest(category, label, iterations);
  }
}

} // namespace test_runner
