#pragma once

#include "math_functions.hpp"
#include "utils.hpp"

namespace runtime_polymorphism {

class Base {
public:
  virtual double Compute(double x) const = 0;
  virtual ~Base() = default;
};

class PolyMinimal : public Base {
public:
  double Compute(double x) const override { return ComputeMinimal(x); }
};

class PolyFMA : public Base {
public:
  double Compute(double x) const override { return ComputeFMA(x); }
};

class PolySimple : public Base {
public:
  double Compute(double x) const override { return ComputeSimple(x); }
};

class PolyMedium : public Base {
public:
  double Compute(double x) const override { return ComputeMedium(x); }
};

class PolyExpensive : public Base {
public:
  double Compute(double x) const override { return ComputeExpensive(x); }
};

void TestRuntimePolymorphism(const std::string &label, size_t n, Base &obj);

} // namespace runtime_polymorphism
