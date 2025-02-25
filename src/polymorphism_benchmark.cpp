#include <cmath>
#include <chrono>
#include <concepts>
#include <iostream>
#include <memory>
#include <vector>

// =================== Runtime Polymorphism (Virtual Functions) ===================

class Base {
 public:
  virtual double Compute(double x) const = 0;
  virtual ~Base() = default;
};

class Derived1 : public Base {
 public:
  double Compute(double x) const override { return x * x + x; }
};

class SmallDerived : public Base {
 public:
  double Compute(double x) const override { return x; }
};

class ExpensiveDerived : public Base {
 public:
  double Compute(double x) const override {
    return std::sin(x) * std::log(x + 1) + std::sqrt(x);
  }
};

// Prevents compiler optimizations
volatile double prevent_optimization = 0.0;

// Generic function to test runtime polymorphism
double TestRuntimePolymorphism(const std::string& label, size_t n, Base& obj) {
  auto start = std::chrono::high_resolution_clock::now();
  double sum = 0.0;
  for (size_t i = 0; i < n; ++i) {
    sum += obj.Compute(2.0);
  }
  prevent_optimization = sum;
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << label << " Runtime Polymorphism Time: " << elapsed.count() << " seconds\n";
  return sum;
}

// =================== Compile-Time Polymorphism (Templates) ===================

template <typename T>
class Compute {
 public:
  double ComputeValue(double x) const { return T::ComputeImpl(x); }
};

struct Squarer {
  static double ComputeImpl(double x) { return x * x + x; }
};

struct Small {
  static double ComputeImpl(double x) { return x; }
};

struct Expensive {
  static double ComputeImpl(double x) {
    return std::sin(x) * std::log(x + 1) + std::sqrt(x);
  }
};

// Generic function to test compile-time polymorphism
template <typename T>
double TestCompileTimePolymorphism(const std::string& label, size_t n) {
  Compute<T> obj;
  auto start = std::chrono::high_resolution_clock::now();
  double sum = 0.0;
  for (size_t i = 0; i < n; ++i) {
    sum += obj.ComputeValue(2.0);
  }
  prevent_optimization = sum;
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << label << " Compile-Time Polymorphism Time: " << elapsed.count() << " seconds\n";
  return sum;
}

// =================== CRTP Compile-Time Polymorphism ===================

template <typename Derived>
class CRTPBase {
 public:
  double Compute(double x) const {
    return static_cast<const Derived*>(this)->ComputeImpl(x);
  }
};

class CRTPDerived1 : public CRTPBase<CRTPDerived1> {
 public:
  double ComputeImpl(double x) const { return x * x + x; }
};

class CRTPSmall : public CRTPBase<CRTPSmall> {
 public:
  double ComputeImpl(double x) const { return x; }
};

class CRTPExpensive : public CRTPBase<CRTPExpensive> {
 public:
  double ComputeImpl(double x) const {
    return std::sin(x) * std::log(x + 1) + std::sqrt(x);
  }
};

// Generic function to test CRTP polymorphism
template <typename T>
double TestCRTPPolymorphism(const std::string& label, size_t n) {
  T obj;
  auto start = std::chrono::high_resolution_clock::now();
  double sum = 0.0;
  for (size_t i = 0; i < n; ++i) {
    sum += obj.Compute(2.0);
  }
  prevent_optimization = sum;
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << label << " CRTP Compile-Time Polymorphism Time: " << elapsed.count() << " seconds\n";
  return sum;
}

// =================== C++20 Concepts-Based Polymorphism ===================

template <typename T>
concept Computable = requires(T t, double x) {
  { t.Compute(x) } -> std::convertible_to<double>;
};

struct ConceptSquarer {
  double Compute(double x) const { return x * x + x; }
};

struct ConceptSmall {
  double Compute(double x) const { return x; }
};

struct ConceptExpensive {
  double Compute(double x) const {
    return std::sin(x) * std::log(x + 1) + std::sqrt(x);
  }
};

// Generic function to test C++20 Concepts polymorphism
template <Computable T>
double TestConceptsPolymorphism(const std::string& label, size_t n, T obj) {
  auto start = std::chrono::high_resolution_clock::now();
  double sum = 0.0;
  for (size_t i = 0; i < n; ++i) {
    sum += obj.Compute(2.0);
  }
  prevent_optimization = sum;
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << label << " C++20 Concepts Polymorphism Time: " << elapsed.count() << " seconds\n";
  return sum;
}

// =================== Main Function ===================
int main() {
  constexpr size_t kNumIterations = 1000000000;  // 1 billion iterations

  std::cout << "\n--- Testing Runtime Polymorphism (Virtual Functions) ---\n";
  Derived1 d1;
  TestRuntimePolymorphism("Simple Computation:", kNumIterations, d1);
  SmallDerived sd;
  TestRuntimePolymorphism("Minimal Work Function:", kNumIterations, sd);
  ExpensiveDerived ed;
  TestRuntimePolymorphism("Expensive Computation:", kNumIterations, ed);

  std::cout << "\n--- Testing Compile-Time Polymorphism (Templates) ---\n";
  TestCompileTimePolymorphism<Squarer>("Simple Computation:", kNumIterations);
  TestCompileTimePolymorphism<Small>("Minimal Work Function:", kNumIterations);
  TestCompileTimePolymorphism<Expensive>("Expensive Computation:", kNumIterations);

  std::cout << "\n--- Testing CRTP Compile-Time Polymorphism ---\n";
  TestCRTPPolymorphism<CRTPDerived1>("Simple Computation:", kNumIterations);
  TestCRTPPolymorphism<CRTPSmall>("Minimal Work Function:", kNumIterations);
  TestCRTPPolymorphism<CRTPExpensive>("Expensive Computation:", kNumIterations);

  std::cout << "\n--- Testing C++20 Concepts-Based Polymorphism ---\n";
  TestConceptsPolymorphism("Simple Computation:", kNumIterations, ConceptSquarer());
  TestConceptsPolymorphism("Minimal Work Function:", kNumIterations, ConceptSmall());
  TestConceptsPolymorphism("Expensive Computation:", kNumIterations, ConceptExpensive());

  return 0;
}
