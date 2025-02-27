#include "polymorphism_tests.hpp"

namespace polymorphism_tests {

// Runtime Polymorphism Tests
void TestRuntimeFMA(size_t iterations) {
  runtime_polymorphism::PolyFMA runtime_fma;
  runtime_polymorphism::TestRuntimePolymorphism(
      "FMA Computation:",
      iterations,
      runtime_fma
  );
}

void TestRuntimeExpensive(size_t iterations) {
  runtime_polymorphism::PolyExpensive runtime_expensive;
  runtime_polymorphism::TestRuntimePolymorphism(
      "Expensive Computation:",
      iterations,
      runtime_expensive
  );
}

// CRTP Polymorphism Tests
void TestCRTPFMA(size_t iterations) {
  crtp_polymorphism::PolyFMA crtp_fma;
  crtp_polymorphism::TestCRTPPolymorphism(
      "FMA Computation:",
      iterations,
      crtp_fma
  );
}

void TestCRTPExpensive(size_t iterations) {
  crtp_polymorphism::PolyExpensive crtp_expensive;
  crtp_polymorphism::TestCRTPPolymorphism(
      "Expensive Computation:",
      iterations,
      crtp_expensive
  );
}

// Concepts Polymorphism Tests
void TestConceptsFMA(size_t iterations) {
  concepts_polymorphism::PolyFMA concepts_fma;
  concepts_polymorphism::TestConceptsPolymorphism(
      "FMA Computation:",
      iterations,
      concepts_fma
  );
}

void TestConceptsExpensive(size_t iterations) {
  concepts_polymorphism::PolyExpensive concepts_expensive;
  concepts_polymorphism::TestConceptsPolymorphism(
      "Expensive Computation:",
      iterations,
      concepts_expensive
  );
}

} // namespace polymorphism_tests
