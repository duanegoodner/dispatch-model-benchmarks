#include "runtime_polymorphism.hpp"

namespace runtime_polymorphism {

void TestRuntimePolymorphism(const std::string& label, size_t n, Base& obj) {
  auto start = std::chrono::high_resolution_clock::now();
  double sum = 0.0;
  for (size_t i = 0; i < n; ++i) {
    sum += obj.Compute(2.0);
  }
  prevent_optimization = sum;
  auto end = std::chrono::high_resolution_clock::now();
  PrintTime(label + " Runtime Polymorphism", end - start);
}

} // namespace runtime_polymorphism
