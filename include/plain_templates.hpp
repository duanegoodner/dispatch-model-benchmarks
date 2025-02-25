#pragma once

#include "utils.hpp"

template <typename T>
class Compute {
 public:
  double ComputeValue(double x) const { return T::ComputeImpl(x); }
};

struct Squarer {
  static double ComputeImpl(double x) { return ComputeSimple(x); }
};

struct Small {
  static double ComputeImpl(double x) { return ComputeMinimal(x); }
};

struct Expensive {
  static double ComputeImpl(double x) {
    return ComputeExpensive(x);
  }
};

template <typename T>
inline void TestPlainTemplates(const std::string& label, size_t n) {
  Compute<T> obj;
  auto start = std::chrono::high_resolution_clock::now();
  double sum = 0.0;
  for (size_t i = 0; i < n; ++i) {
    sum += obj.ComputeValue(2.0);
  }
  prevent_optimization = sum;
  auto end = std::chrono::high_resolution_clock::now();
  PrintTime(label + " Plain Templates", end - start);
}
