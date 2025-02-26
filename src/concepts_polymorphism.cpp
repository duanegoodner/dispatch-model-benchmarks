#include "benchmark_utils.hpp"
#include "concepts_polymorphism.hpp"

namespace concepts_polymorphism {

double PolyFMA::Compute(double x) const {return ComputeFMA(x);}

double PolyExpensive::Compute(double x) const {return ComputeExpensive(x);}

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
