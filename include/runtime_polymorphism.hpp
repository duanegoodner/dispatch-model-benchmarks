// Base class and declarations needed for runtime polymorphism.

#pragma once

#include "benchmark_utils.hpp"
#include "math_functions.hpp"

namespace runtime_polymorphism {

class RuntimeBase {
public:
  virtual double Compute(double x) const = 0;
  virtual ~RuntimeBase() = default;
};

class PolyFMA : public RuntimeBase {
public:
  double Compute(double x) const override;
};

class PolyExpensive : public RuntimeBase {
public:
  double Compute(double x) const override;
};

void TestRuntimePolymorphism(const std::string &label, size_t n, RuntimeBase &obj);

} // namespace runtime_polymorphism
