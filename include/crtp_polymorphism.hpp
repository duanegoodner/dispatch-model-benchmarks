#pragma once

#include "utils.hpp"

template <typename Derived>
class CRTPBase {
public:
  double Compute(double x) const {
    return static_cast<const Derived *>(this)->ComputeImpl(x);
  }
};

class CRTPDerived1 : public CRTPBase<CRTPDerived1> {
public:
  double ComputeImpl(double x) const { return ComputeSimple(x); }
};

class CRTPSmall : public CRTPBase<CRTPSmall> {
public:
  double ComputeImpl(double x) const { return ComputeMinimal(x); }
};

class CRTPExpensive : public CRTPBase<CRTPExpensive> {
public:
  double ComputeImpl(double x) const { return ComputeExpensive(x); }
};

template <typename T>
inline void TestCRTPPolymorphism(const std::string &label, size_t n) {
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
