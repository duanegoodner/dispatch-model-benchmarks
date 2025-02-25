#pragma once

#include <concepts>
#include "utils.hpp"
#include "math_functions.hpp"

namespace concepts_polymorphism {

template <typename T>
concept Computable = requires(T t, double x) {
  { t.Compute(x) } -> std::convertible_to<double>;
};

struct PolyMinimal {
  double Compute(double x) const { return ComputeMinimal(x); }
};

struct PolyFMA {
  double Compute(double x) const { return ComputeFMA(x); }
};


struct PolySimple {
  double Compute(double x) const { return ComputeSimple(x); }
};

struct PolyMedium {
  double Compute(double x) const { return ComputeMedium(x); }
};

struct PolyExpensive {
  double Compute(double x) const { return ComputeExpensive(x); }
};

template <Computable T>
void TestConceptsPolymorphism(const std::string& label, size_t n, T obj);

} // namespace concepts_polymorphism
