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
  PolyFMA crtpFMA;
  PolyExpensive crtpExpensive;

  TestCRTPPolymorphism<PolyFMA>(testLabels[0], iterations, crtpFMA);
  TestCRTPPolymorphism<PolyExpensive>(testLabels[1], iterations, crtpExpensive);
}
} // namespace crtp_polymorphism

namespace concepts_polymorphism {
void RunTests(size_t iterations) {
  PolyFMA conceptsFMA;
  PolyExpensive conceptsExppensive;
  
  TestConceptsPolymorphism(testLabels[0], iterations, conceptsFMA);
  TestConceptsPolymorphism(testLabels[1], iterations, conceptsExppensive);
}
} // namespace concepts_polymorphism
