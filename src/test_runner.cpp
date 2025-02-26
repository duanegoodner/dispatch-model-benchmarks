#include "test_runner.hpp"
#include <string>
#include <unordered_map>
#include <vector>

static const std::vector<std::string> testLabels = {
    "FMA Computation:",
    "Expensive Computation:"};

namespace runtime_polymorphism {
void TestFMA(size_t iterations) {
  PolyFMA runtime_fma;
  TestRuntimePolymorphism("FMA Computation:", iterations, runtime_fma);
}

void TestExpensive(size_t iterations) {
  PolyExpensive runtime_expensive;
  TestRuntimePolymorphism(
      "Expensive Computation:",
      iterations,
      runtime_expensive
  );
}

} // namespace runtime_polymorphism

namespace crtp_polymorphism {

void TestFMA(size_t iterations) {
  PolyFMA crtp_fma;
  TestCRTPPolymorphism("FMA Computation:", iterations, crtp_fma);
}

void TestExpensive(size_t iterations) {
  PolyExpensive crtp_expensive;
  TestCRTPPolymorphism("Expensive Computation:", iterations, crtp_expensive);
}

} // namespace crtp_polymorphism

namespace concepts_polymorphism {

void TestFMA(size_t iterations) {
  PolyFMA concepts_fma;
  TestConceptsPolymorphism("FMA Computation:", iterations, concepts_fma);
}
void TestExpensive(size_t iterations) {
  PolyExpensive concepts_expensive;
  TestConceptsPolymorphism(
      "Expensive Computation:",
      iterations,
      concepts_expensive
  );
}
} // namespace concepts_polymorphism

// Function to retrieve a single test case based on namespace and computation
// name
const TestCase &GetSingleTestCase(
    const std::string &ns,
    const std::string &computation
) {
  static const std::unordered_map<
      std::string,
      std::unordered_map<std::string, TestCase>>
      testCaseMap = {
          {"runtime",
           {{"fma",
             {"runtime_polymorphism::TestFMA", runtime_polymorphism::TestFMA}},
            {"expensive",
             {"runtime_polymorphism::TestExpensive",
              runtime_polymorphism::TestExpensive}}}},
          {"crtp",
           {{"fma", {"crtp_polymorphism::TestFMA", crtp_polymorphism::TestFMA}},
            {"expensive",
             {"crtp_polymorphism::TestExpensive",
              crtp_polymorphism::TestExpensive}}}},
          {"concepts",
           {{"fma",
             {"concepts_polymorphism::TestFMA",
              concepts_polymorphism::TestFMA}},
            {"expensive",
             {"concepts_polymorphism::TestExpensive",
              concepts_polymorphism::TestExpensive}}}}};

  // Look up the namespace
  auto nsIt = testCaseMap.find(ns);
  if (nsIt == testCaseMap.end()) {
    throw std::invalid_argument("Invalid namespace: " + ns);
  }

  // Look up the computation type
  auto compIt = nsIt->second.find(computation);
  if (compIt == nsIt->second.end()) {
    throw std::invalid_argument("Invalid computation: " + computation);
  }

  return compIt->second; // Return the found TestCase
}

void RunSingleTest(
    const std::string &polymorphism_category,
    const std::string &computation_label,
    size_t iterations
) {
  auto &test_case = GetSingleTestCase(polymorphism_category, computation_label);
  std::cout << "Running: " << test_case.name << std::endl;
  test_case.function(iterations);
}

void RunAllTests(size_t iterations) {
  // Define the valid pairs of (polymorphism_category, computation_label)
  static const std::vector<std::pair<std::string, std::string>> testPairs = {
      {"runtime", "fma"},
      {"runtime", "expensive"},
      {"crtp", "fma"},
      {"crtp", "expensive"},
      {"concepts", "fma"},
      {"concepts", "expensive"}
  };

  // Loop through each valid pair and run the test
  for (const auto& [category, label] : testPairs) {
      RunSingleTest(category, label, iterations);
  }
}
