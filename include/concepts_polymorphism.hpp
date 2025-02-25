#pragma once

#include "utils.hpp"

template <typename T>
concept Computable = requires(T t, double x) {
  { t.Compute(x) } -> std::convertible_to<double>;
};

struct ConceptSquarer {
  double Compute(double x) const { return ComputeSimple(x);  }
};

struct ConceptSmall {
  double Compute(double x) const { return ComputeMinimal(x);  }
};

struct ConceptExpensive {
  double Compute(double x) const {
    return ComputeExpensive(x);
  }
};

template <Computable T>
inline void TestConceptsPolymorphism(const std::string& label, size_t n, T obj) {
  auto start = std::chrono::high_resolution_clock::now();
  double sum = 0.0;
  for (size_t i = 0; i < n; ++i) {
    sum += obj.Compute(2.0);
  }
  prevent_optimization = sum;
  auto end = std::chrono::high_resolution_clock::now();
  PrintTime(label + " C++20 Concepts Polymorphism", end - start);
}
