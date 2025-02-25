#pragma once

#include <cmath>

// Simple computation: x² + x
inline double ComputeSimple(double x) {
    return x * x + x;
}

// Minimal work: return x
inline double ComputeMinimal(double x) {
    return x;
}

// Expensive computation: sin(x) * log(x+1) + sqrt(x)
inline double ComputeExpensive(double x) {
    return std::sin(x) * std::log(x + 1) + std::sqrt(x);
}
