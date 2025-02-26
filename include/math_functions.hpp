// Contains inlined computation functions that will be tested across different
// types of polymorphism.

#pragma once

#include <cmath>

// Fused Multiply-Add: x * 1.414 + 2.718 (Approximates sqrt(2) and e)
inline double ComputeFMA(double x) { return x * 1.414 + 2.718; }

// Expensive computation: sin(x) * log(x+1) + sqrt(x)
inline double ComputeExpensive(double x) {
  return std::sin(x) * std::log(x + 1) + std::sqrt(x);
}
