#include <iostream>
#include "concepts_polymorphism.hpp"
#include "crtp_polymorphism.hpp"
#include "plain_templates.hpp"
#include "runtime_polymorphism.hpp"

int main() {
  constexpr size_t kNumIterations = 1000000000;

  // Runtime Polymorphism
  runtime_polymorphism::PolyMinimal runtimeMinimal;
  runtime_polymorphism::PolyFMA runtimeFMA;
  runtime_polymorphism::PolyMedium runtimeMedium;
  runtime_polymorphism::PolySimple runtimeSimple;
  runtime_polymorphism::PolyExpensive runtimeExpensive;

  TestRuntimePolymorphism(
      "Minimal Work Function:",
      kNumIterations,
      runtimeMinimal
  );
  TestRuntimePolymorphism("Simple Computation:", kNumIterations, runtimeSimple);
  TestRuntimePolymorphism("FMA Computation:", kNumIterations, runtimeFMA);
  TestRuntimePolymorphism("Medium Computation:", kNumIterations, runtimeMedium);
  TestRuntimePolymorphism(
      "Expensive Computation:",
      kNumIterations,
      runtimeExpensive
  );
  std::cout << std::endl;

  // Plain Templates

  plain_templates::TestPlainTemplates<plain_templates::PolyMinimal>(
      "Minimal Work Function:",
      kNumIterations
  );

  plain_templates::TestPlainTemplates<plain_templates::PolyFMA>(
      "FMA Computation::",
      kNumIterations
  );

  plain_templates::TestPlainTemplates<plain_templates::PolySimple>(
      "Simple Computation:",
      kNumIterations
  );
  plain_templates::TestPlainTemplates<plain_templates::PolyMedium>(
      "Medium Computation:",
      kNumIterations
  );
  plain_templates::TestPlainTemplates<plain_templates::PolyExpensive>(
      "Expensive Computation:",
      kNumIterations
  );
  std::cout << std::endl;



  // CRTP

  crtp_polymorphism::TestCRTPPolymorphism<crtp_polymorphism::PolyMinimal>(
      "Minimal Work Function:",
      kNumIterations
  );

  crtp_polymorphism::TestCRTPPolymorphism<crtp_polymorphism::PolyFMA>(
      "FMA Computation:",
      kNumIterations
  );
  crtp_polymorphism::TestCRTPPolymorphism<crtp_polymorphism::PolySimple>(
      "Simple Computation:",
      kNumIterations
  );
  crtp_polymorphism::TestCRTPPolymorphism<crtp_polymorphism::PolyMedium>(
      "Medium Computation:",
      kNumIterations
  );
  crtp_polymorphism::TestCRTPPolymorphism<crtp_polymorphism::PolyExpensive>(
      "Expensive Computation:",
      kNumIterations
  );
  std::cout << std::endl;

  // Concepts-Based
  TestConceptsPolymorphism(
      "Minimal Work Function:",
      kNumIterations,
      concepts_polymorphism::PolyMinimal()
  );
  TestConceptsPolymorphism(
      "FMA Computation:",
      kNumIterations,
      concepts_polymorphism::PolyFMA()
  );
  TestConceptsPolymorphism(
      "Simple Computation:",
      kNumIterations,
      concepts_polymorphism::PolySimple()
  );
  TestConceptsPolymorphism(
      "Medium Computation:",
      kNumIterations,
      concepts_polymorphism::PolyMedium()
  );
  TestConceptsPolymorphism(
      "Expensive Computation:",
      kNumIterations,
      concepts_polymorphism::PolyExpensive()
  );
  std::cout << std::endl;

  return 0;
}
