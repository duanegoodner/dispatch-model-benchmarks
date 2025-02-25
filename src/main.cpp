#include "runtime_polymorphism.hpp"
#include "plain_templates.hpp"
#include "crtp_polymorphism.hpp"
#include "concepts_polymorphism.hpp"

int main() {
  constexpr size_t kNumIterations = 1000000000;

  Derived1 d1;
  TestRuntimePolymorphism("Simple Computation:", kNumIterations, d1);
  TestPlainTemplates<Squarer>("Simple Computation:", kNumIterations);
  TestCRTPPolymorphism<CRTPDerived1>("Simple Computation:", kNumIterations);
  TestConceptsPolymorphism("Simple Computation:", kNumIterations, ConceptSquarer());

  return 0;
}
