#pragma once

#include <cmath>

// Minimal work: return x
inline double ComputeMinimal(double x) {
    return x;
}

// Fused Multiply-Add: x * 1.414 + 2.718 (Approximates sqrt(2) and e)
inline double ComputeFMA(double x) {
    return x * 1.414 + 2.718;
}

// Simple computation: x² + x
inline double ComputeSimple(double x) {
    return x * x + x;
}

// Medium complexity: x³ + 2x² - 3x + 5 (Cubic Polynomial)
inline double ComputeMedium(double x) {
    return (x * x * x) + (2.0 * x * x) - (3.0 * x) + 5.0;
}

// Expensive computation: sin(x) * log(x+1) + sqrt(x)
inline double ComputeExpensive(double x) {
    return std::sin(x) * std::log(x + 1) + std::sqrt(x);
}
