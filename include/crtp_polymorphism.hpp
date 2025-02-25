#pragma once

#include "utils.hpp"
#include "math_functions.hpp"

namespace crtp_polymorphism {

template <typename Derived>
class CRTPBase {
 public:
  double Compute(double x) const {
    return static_cast<const Derived*>(this)->ComputeImpl(x);
  }
};

class PolyMinimal : public CRTPBase<PolyMinimal> {
  public:
   double ComputeImpl(double x) const { return ComputeMinimal(x); }
 };

 class PolyFMA : public CRTPBase<PolyFMA> {
  public:
   double ComputeImpl(double x) const { return ComputeFMA(x); }
 };
 

class PolySimple : public CRTPBase<PolySimple> {
 public:
  double ComputeImpl(double x) const { return ComputeSimple(x); }
};

class PolyMedium : public CRTPBase<PolySimple> {
  public:
   double ComputeImpl(double x) const { return ComputeMedium(x); }
 };
 

class PolyExpensive : public CRTPBase<PolyExpensive> {
 public:
  double ComputeImpl(double x) const { return ComputeExpensive(x); }
};

template <typename T>
void TestCRTPPolymorphism(const std::string& label, size_t n);

} // namespace crtp_polymorphism
