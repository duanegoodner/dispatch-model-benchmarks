#include <string>
#include <vector>
#include "test_runner.hpp"

static const std::vector<std::string> testLabels = {
    "FMA Computation:",
    "Expensive Computation:"
};

namespace runtime_polymorphism {
void RunTests(size_t iterations) {
  PolyFMA runtimeFMA;
  PolyExpensive runtimeExpensive;

  TestRuntimePolymorphism(testLabels[0], iterations, runtimeFMA);
  TestRuntimePolymorphism(testLabels[1], iterations, runtimeExpensive);
}
} // namespace runtime_polymorphism

namespace crtp_polymorphism {
void RunTests(size_t iterations) {
  TestCRTPPolymorphism<PolyFMA>(testLabels[0], iterations);
  TestCRTPPolymorphism<PolyExpensive>(testLabels[1], iterations);
}
} // namespace crtp_polymorphism

namespace concepts_polymorphism {
void RunTests(size_t iterations) {
  TestConceptsPolymorphism(testLabels[0], iterations, PolyFMA());
  TestConceptsPolymorphism(testLabels[1], iterations, PolyExpensive());
}
} // namespace concepts_polymorphism
