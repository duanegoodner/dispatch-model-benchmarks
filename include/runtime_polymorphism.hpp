#pragma once

#include "math_functions.hpp"
#include "utils.hpp"

class Base {
public:
  virtual double Compute(double x) const = 0;
  virtual ~Base() = default;
};

class Derived1 : public Base {
public:
  double Compute(double x) const override { return ComputeSimple(x); }
};

class SmallDerived : public Base {
public:
  double Compute(double x) const override { return ComputeMinimal(x); }
};

class ExpensiveDerived : public Base {
public:
  double Compute(double x) const override { return ComputeExpensive(x); }
};

inline void TestRuntimePolymorphism(
    const std::string &label,
    size_t n,
    Base &obj
) {
  auto start = std::chrono::high_resolution_clock::now();
  double sum = 0.0;
  for (size_t i = 0; i < n; ++i) {
    sum += obj.Compute(2.0);
  }
  prevent_optimization = sum;
  auto end = std::chrono::high_resolution_clock::now();
  PrintTime(label + " Runtime Polymorphism", end - start);
}
