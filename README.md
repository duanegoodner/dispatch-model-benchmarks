# polymorphism-compare
Benchmarking Runtime vs Compile-Time Polymorphism for Different Compute Functions in C++

## Background
When discussing **compile-time vs runtime polymorphism** in C++, it’s often stated that compile-time polymorphism can be faster due to factors like function inlining, elimination of virtual table lookups, and better compiler optimizations. However, these discussions are frequently followed by a caveat: The actual performance difference depends on many factors, and profiling is necessary to determine whether compile-time polymorphism provides a meaningful advantage in any given use case.

As I thought about this, I started wondering: what specific comparisons had actually been reported? I expected to find the web, GitHub, and academic literature filled with benchmark results from developers evaluating whether compile-time polymorphism would benefit their specific use case. But after searching, I realized that **hard numbers were surprisingly difficult to find**. Most discussions acknowledge the potential differences, but very few provide concrete performance data.

# What This Project Does

## Compares Runtime vs Compile-Time Polymorphism for Two Specific Functions

This project benchmarks runtime vs compile-time polymorphism in C++ by comparing execution times for two compute functions:
- A **simple computation** – Fused Multiply-Add (FMA):
```
inline double ComputeFMA(double x) { return x * 1.414 + 2.718; }
```
- And a (comparatively) **complex computation** inolving Sin, Log and a Square Root:
```
inline double ComputeExpensive(double x) {
    return std::sin(x) * std::log(x + 1) + std::sqrt(x);
}
```
Each function is tested using:
- **Runtime polymorphism** via virtual function calls
- **Compile-time polymorphism** using the Curiously Recurring Template Pattern (CRTP)
- **C++20 Concepts**  which enable **compile-time type enforcement** and selection, achieving behavior similar to polymorphism without inheritance.

## Provides a Template for Benchmarking Other Compute Functions

This project is designed with modularity and ease of modification in mind, making it a useful starting point for benchmarking compile-time vs. runtime polymorphism with other compute functions. The codebase provides a structured comparison framework that can be easily adapted to test additional operations.