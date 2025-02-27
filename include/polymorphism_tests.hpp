// Declarations for test functions. Each test is for a particular combination
// of polymorphism type and compute function.

#pragma once

#include "runtime_polymorphism.hpp"
#include "crtp_polymorphism.hpp"
#include "concepts_polymorphism.hpp"

namespace polymorphism_tests {

void TestRuntimeFMA(size_t iterations);
void TestRuntimeExpensive(size_t iterations);

void TestCRTPFMA(size_t iterations);
void TestCRTPExpensive(size_t iterations);

void TestConceptsFMA(size_t iterations);
void TestConceptsExpensive(size_t iterations);

}  // namespace polymorphism_tests
