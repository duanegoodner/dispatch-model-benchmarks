#include "utils.hpp"
#include "concepts_polymorphism.hpp"

namespace concepts_polymorphism {

template <Computable T>
void TestConceptsPolymorphism(const std::string &label, size_t n, T obj) {
  RunBenchmark(label + " C++20 Concepts Polymorphism", n, [&](double x) {
    return obj.Compute(x);
  });
}

// Explicit template instantiations
template void TestConceptsPolymorphism<PolyFMA>(
    const std::string &,
    size_t,
    PolyFMA
);

template void TestConceptsPolymorphism<PolyExpensive>(
    const std::string &,
    size_t,
    PolyExpensive
);

} // namespace concepts_polymorphism
