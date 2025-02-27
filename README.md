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


## Building

If we want to use the default compiler flags `-O3 -march=native` specified by our `CMakeLists.txt`, we can run:
```shell
https://github.com/duanegoodner/polymorphism-compare
cd polymorphism-compare
mkdir build
cmake -B build
cmake --build build
```
The `CMakeList.txt` file also offers the following options:

| -D Option         | Compiler Flags |
|-------------------|---------------------------------|
| ENABLE_DEBUG      | `-O0 -g`                        |
| ENABLE_NO_INLINE  | `-O3 -march=native -fno-inline` |
| ENABLE_LOW_OPT    | `-O1 -march=native`             |
| ENABLE_PROFILING  | `-O3 -march=native -pg`         |
| RESET_DEFAULTS    | `-O3 -march=native`             |

For example, if we want to use compiler flags `-O3 -march=native -pg`, then instead of `cmake -B build` in, we would use `cmake -B build -DENABLE_PROFILING=ON`.


## Running

### Command Line Help

```shell
./build/bin/benchmark --help

# Output:
# Usage: ./build/bin/benchmark [polymorphism_category] [computation]
#  - No arguments: Runs all tests.
#  - With arguments: Runs a specific test.
#    Valid values for polymorphism_category: concepts crtp runtime 
#    Valid values for computation: expensive fma 
```

### Run All Compute Functions with All Polymorphism Variants

To run all possible combinations of polymorphism type and compute function, run the executable file without any arguments. 

```shell
./build/bin/benchmark

# Output:
# Running: polymorphism_tests::TestRuntimeFMA
# FMA Computation: Runtime Polymorphism Time = 1.5178 seconds
# 
# Running: polymorphism_tests::TestRuntimeExpensive
# Expensive Computation: Runtime Polymorphism Time = 6.31193 seconds
# 
# Running: polymorphism_tests::TestCRTPFMA
# FMA Computation: CRTP Polymorphism Time = 0.372842 seconds
# 
# Running: polymorphism_tests::TestCRTPExpensive
# Expensive Computation: CRTP Polymorphism Time = 0.372428 seconds
# 
# Running: polymorphism_tests::TestConceptsFMA
# FMA Computation: C++20 Concepts Polymorphism Time = 0.373231 seconds
# 
# Running: polymorphism_tests::TestConceptsExpensive
# Expensive Computation: C++20 Concepts Polymorphism Time = 0.374191 seconds
# 
# Test results saved to: data/run_all_tests_results/2025-02-26-23-53-19-980.txt
```

### Running a Single Compute Function with a Single Polymorphism Variant

To run a single compute function with a single type of polymorphism, the command syntax is:
```shell
./build/bin/benchmark <polymorphism_category> <computation>
```
For example:
```
./build/bin/benchmark crtp expensive

# Output:
# Running: polymorphism_tests::TestCRTPExpensive
# Expensive Computation: CRTP Polymorphism Time = 0.379541 seconds
```

## Results









