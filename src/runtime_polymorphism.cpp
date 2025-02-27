#include "runtime_polymorphism.hpp"
#include "benchmark_utils.hpp"

namespace runtime_polymorphism {

// Implement PolyFMA::Compute
double PolyFMA::Compute(double x) const { return ComputeFMA(x); }

// Implement PolyExpensive::Compute
double PolyExpensive::Compute(double x) const { return ComputeExpensive(x); }

// Implement TestRuntimePolymorphism
void TestRuntimePolymorphism(const std::string &label, size_t n, RuntimeBase &obj) {
  RunBenchmark(label + " Runtime Polymorphism", n, [&](double x) {
    return obj.Compute(x);
  });
}

} // namespace runtime_polymorphism
