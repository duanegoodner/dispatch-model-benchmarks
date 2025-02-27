#include "crtp_polymorphism.hpp"
#include "benchmark_utils.hpp"

namespace crtp_polymorphism {

// Explicit template instantiations of concrete CRTP classes

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
