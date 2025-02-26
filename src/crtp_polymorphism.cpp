#include "crtp_polymorphism.hpp"
#include "benchmark_utils.hpp"

namespace crtp_polymorphism {

template <typename T>
void TestCRTPPolymorphism(const std::string &label, size_t n, T &obj) {
  RunBenchmark(label + " CRTP Polymorphism", n, [&](double x) {
    return obj.Compute(x);
  });
}

// Explicit template instantiations
template void TestCRTPPolymorphism<PolyFMA>(
    const std::string &label,
    size_t n,
    PolyFMA &obj
);
template void TestCRTPPolymorphism<PolyExpensive>(
    const std::string &label,
    size_t n,
    PolyExpensive &obj
);

} // namespace crtp_polymorphism
