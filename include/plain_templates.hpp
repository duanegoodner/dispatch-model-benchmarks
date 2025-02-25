#pragma once

#include "math_functions.hpp"
#include "utils.hpp"

namespace plain_templates {

template <typename T>
class Compute {
public:
  double ComputeValue(double x) const { return T::ComputeImpl(x); }
};

struct PolyMinimal {
  static double ComputeImpl(double x) { return ComputeMinimal(x); }
};

struct PolyFMA {
  static double ComputeImpl(double x) { return ComputeFMA(x); }
};


struct PolySimple {
  static double ComputeImpl(double x) { return ComputeSimple(x); }
};

struct PolyMedium {
  static double ComputeImpl(double x) { return ComputeMedium(x); }
};

struct PolyExpensive {
  static double ComputeImpl(double x) { return ComputeExpensive(x); }
};

template <typename T>
void TestPlainTemplates(const std::string &label, size_t n);

} // namespace plain_templates
