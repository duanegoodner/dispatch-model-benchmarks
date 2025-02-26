#include <string>
#include <vector>
#include "test_runner.hpp"

static const std::vector<std::string> testLabels = {
    "Minimal Work Function:",
    "FMA Computation:",
    "Simple Computation:",
    "Medium Computation:",
    "Expensive Computation:"
};

namespace runtime_polymorphism {
void RunTests(size_t iterations) {
  PolyMinimal runtimeMinimal;
  PolyFMA runtimeFMA;
  PolySimple runtimeSimple;
  PolyMedium runtimeMedium;
  PolyExpensive runtimeExpensive;

  TestRuntimePolymorphism(testLabels[0], iterations, runtimeMinimal);
  TestRuntimePolymorphism(testLabels[1], iterations, runtimeFMA);
  TestRuntimePolymorphism(testLabels[2], iterations, runtimeSimple);
  TestRuntimePolymorphism(testLabels[3], iterations, runtimeMedium);
  TestRuntimePolymorphism(testLabels[4], iterations, runtimeExpensive);
}
} // namespace runtime_polymorphism

namespace plain_templates {
void RunTests(size_t iterations) {
  TestPlainTemplates<PolyMinimal>(testLabels[0], iterations);
  TestPlainTemplates<PolyFMA>(testLabels[1], iterations);
  TestPlainTemplates<PolySimple>(testLabels[2], iterations);
  TestPlainTemplates<PolyMedium>(testLabels[3], iterations);
  TestPlainTemplates<PolyExpensive>(testLabels[4], iterations);
}
} // namespace plain_templates

namespace crtp_polymorphism {
void RunTests(size_t iterations) {
  TestCRTPPolymorphism<PolyMinimal>(testLabels[0], iterations);
  TestCRTPPolymorphism<PolyFMA>(testLabels[1], iterations);
  TestCRTPPolymorphism<PolySimple>(testLabels[2], iterations);
  TestCRTPPolymorphism<PolyMedium>(testLabels[3], iterations);
  TestCRTPPolymorphism<PolyExpensive>(testLabels[4], iterations);
}
} // namespace crtp_polymorphism

namespace concepts_polymorphism {
void RunTests(size_t iterations) {
  TestConceptsPolymorphism(testLabels[0], iterations, PolyMinimal());
  TestConceptsPolymorphism(testLabels[1], iterations, PolyFMA());
  TestConceptsPolymorphism(testLabels[2], iterations, PolySimple());
  TestConceptsPolymorphism(testLabels[3], iterations, PolyMedium());
  TestConceptsPolymorphism(testLabels[4], iterations, PolyExpensive());
}
} // namespace concepts_polymorphism
