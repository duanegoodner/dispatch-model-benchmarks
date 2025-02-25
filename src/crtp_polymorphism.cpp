#include "crtp_polymorphism.hpp"

namespace crtp_polymorphism {

template <typename T>
void TestCRTPPolymorphism(const std::string& label, size_t n) {
  T obj;
  auto start = std::chrono::high_resolution_clock::now();
  double sum = 0.0;
  for (size_t i = 0; i < n; ++i) {
    sum += obj.Compute(2.0);
  }
  prevent_optimization = sum;
  auto end = std::chrono::high_resolution_clock::now();
  PrintTime(label + " CRTP Polymorphism", end - start);
}

// Explicit template instantiations
template void TestCRTPPolymorphism<PolySimple>(const std::string&, size_t);
template void TestCRTPPolymorphism<PolyFMA>(const std::string&, size_t);
template void TestCRTPPolymorphism<PolyMinimal>(const std::string&, size_t);
template void TestCRTPPolymorphism<PolyMedium>(const std::string&, size_t);
template void TestCRTPPolymorphism<PolyExpensive>(const std::string&, size_t);

} // namespace crtp_polymorphism
