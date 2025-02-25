#pragma once

#include "math_functions.hpp"
#include "utils.hpp"

namespace concepts_polymorphism {

template <typename T>
concept Computable = requires(T t, double x) {
  { t.Compute(x) } -> std::convertible_to<double>;
};

class PolyMinimal {
public:
  double Compute(double x) const { return ComputeMinimal(x); }
};

class PolyFMA {
public:
  double Compute(double x) const { return ComputeFMA(x); }
};

class PolySimple {
public:
  double Compute(double x) const { return ComputeSimple(x); }
};

class PolyMedium {
public:
  double Compute(double x) const { return ComputeMedium(x); }
};

class PolyExpensive {
public:
  double Compute(double x) const { return ComputeExpensive(x); }
};

template <Computable T>
void TestConceptsPolymorphism(const std::string &label, size_t n, T obj);

} // namespace concepts_polymorphism