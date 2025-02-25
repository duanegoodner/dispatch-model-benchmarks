#include "benchmark_utils.hpp"
#include "plain_templates.hpp"

namespace plain_templates {

template <typename T>
void TestPlainTemplates(const std::string &label, size_t n) {
  RunBenchmark(label + " Plain Templates", n, [](double x) {
    return T::ComputeImpl(x);
  });
}

// Explicit template instantiations so they are compiled
template void TestPlainTemplates<PolyMinimal>(const std::string &, size_t);
template void TestPlainTemplates<PolyFMA>(const std::string &, size_t);
template void TestPlainTemplates<PolySimple>(const std::string &, size_t);
template void TestPlainTemplates<PolyMedium>(const std::string &, size_t);
template void TestPlainTemplates<PolyExpensive>(const std::string &, size_t);

} // namespace plain_templates
