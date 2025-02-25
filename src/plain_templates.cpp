#include "plain_templates.hpp"

namespace plain_templates {

template <typename T>
void TestPlainTemplates(const std::string& label, size_t n) {
  Compute<T> obj;
  auto start = std::chrono::high_resolution_clock::now();
  double sum = 0.0;
  for (size_t i = 0; i < n; ++i) {
    sum += obj.ComputeValue(2.0);
  }
  prevent_optimization = sum;
  auto end = std::chrono::high_resolution_clock::now();
  PrintTime(label + " Plain Templates", end - start);
}

// Explicit template instantiations so they are compiled
template void TestPlainTemplates<PolyMinimal>(const std::string&, size_t);
template void TestPlainTemplates<PolyFMA>(const std::string&, size_t);
template void TestPlainTemplates<PolySimple>(const std::string&, size_t);
template void TestPlainTemplates<PolyMedium>(const std::string&, size_t);
template void TestPlainTemplates<PolyExpensive>(const std::string&, size_t);

} // namespace plain_templates
