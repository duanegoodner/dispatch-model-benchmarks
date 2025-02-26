#include "benchmark_utils.hpp"
#include "concepts_polymorphism.hpp"

namespace concepts_polymorphism {

template <Computable T>
void TestConceptsPolymorphism(const std::string &label, size_t n, T &obj) {
  RunBenchmark(label + " C++20 Concepts Polymorphism", n, [&](double x) {
    return obj.Compute(x);
  });
}

// Explicit template instantiations
template void TestConceptsPolymorphism<PolyFMA>(
    const std::string &label,
    size_t n,
    PolyFMA &obj
);

template void TestConceptsPolymorphism<PolyExpensive>(
    const std::string &label,
    size_t n,
    PolyExpensive &obj
);

} // namespace concepts_polymorphism
