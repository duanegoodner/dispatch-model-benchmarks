#include "runtime_polymorphism.hpp"
#include "benchmark_utils.hpp"

namespace runtime_polymorphism {

void TestRuntimePolymorphism(const std::string &label, size_t n, Base &obj) {
  RunBenchmark(label + " Runtime Polymorphism", n, [&](double x) {
    return obj.Compute(x);
  });
}

} // namespace runtime_polymorphism
