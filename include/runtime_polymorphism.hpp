#pragma once

#include "math_functions.hpp"
#include "utils.hpp"

namespace runtime_polymorphism {

class Base {
public:
  virtual double Compute(double x) const = 0;
  virtual ~Base() = default;
};

class PolyFMA : public Base {
public:
  double Compute(double x) const override { return ComputeFMA(x); }
};

class PolyExpensive : public Base {
public:
  double Compute(double x) const override { return ComputeExpensive(x); }
};

void TestRuntimePolymorphism(const std::string &label, size_t n, Base &obj);

} // namespace runtime_polymorphism
