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
  double Compute(double x) const;
};

class PolyExpensive {
public:
  double Compute(double x) const;
};

template <Computable T>
void TestConceptsPolymorphism(const std::string &label, size_t n, T &obj) {
  RunBenchmark(label + " C++20 Concepts Polymorphism", n, [&](double x) {
    return obj.Compute(x);
  });
}

} // namespace concepts_polymorphism