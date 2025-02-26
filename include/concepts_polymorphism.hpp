#pragma once

#include "math_functions.hpp"
#include "benchmark_utils.hpp"

namespace concepts_polymorphism {

template <typename T>
concept Computable = requires(T t, double x) {
  { t.Compute(x) } -> std::convertible_to<double>;
};

class PolyFMA {
public:
  double Compute(double x) const { return ComputeFMA(x); }
};

class PolyExpensive {
public:
  double Compute(double x) const { return ComputeExpensive(x); }
};

template <Computable T>
void TestConceptsPolymorphism(const std::string &label, size_t n, T &obj);

} // namespace concepts_polymorphism