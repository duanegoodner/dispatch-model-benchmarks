#pragma once

#include "math_functions.hpp"
#include "utils.hpp"

namespace plain_templates {

template <typename T>
class Compute {
public:
  double ComputeValue(double x) const { return T::ComputeImpl(x); }
};


class PolyFMA {
public:
  static double ComputeImpl(double x) { return ComputeFMA(x); }
};

class PolyExpensive {
public:
  static double ComputeImpl(double x) { return ComputeExpensive(x); }
};

template <typename T>
void TestPlainTemplates(const std::string &label, size_t n);

} // namespace plain_templates
