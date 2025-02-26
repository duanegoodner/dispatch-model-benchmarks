#pragma once

#include "benchmark_utils.hpp"
#include "math_functions.hpp"

namespace runtime_polymorphism {

class Base {
public:
  virtual double Compute(double x) const = 0;
  virtual ~Base() = default;
};

class PolyFMA : public Base {
public:
  double Compute(double x) const override;
};

class PolyExpensive : public Base {
public:
  double Compute(double x) const override;
};

void TestRuntimePolymorphism(const std::string &label, size_t n, Base &obj);

} // namespace runtime_polymorphism
