# polymorphism-compare
Benchmarking Runtime vs Compile-Time Polymorphism for Different Compute Functions in C++

## Background
When discussing **compile-time vs runtime polymorphism** in C++, it’s often stated that compile-time polymorphism can be faster due to factors like function inlining, elimination of virtual table lookups, and better compiler optimizations. However, these discussions are frequently followed by a caveat: The actual performance difference depends on many factors, and profiling is necessary to determine whether compile-time polymorphism provides a meaningful advantage in any given use case.

As I thought about this, I started wondering: what specific comparisons had actually been reported? I expected to find the web, GitHub, and academic literature filled with benchmark results from developers evaluating whether compile-time polymorphism would benefit their specific use case. But after searching, I realized that **hard numbers were surprisingly difficult to find**. Most discussions acknowledge the potential differences, but very few provide concrete performance data.

## What This Project Does

### Compares Runtime vs Compile-Time Polymorphism for Two Specific Functions

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

### Provides a Template for Benchmarking Other Compute Functions

This project is designed with modularity and ease of modification in mind, making it a useful starting point for benchmarking compile-time vs. runtime polymorphism with other compute functions. The codebase provides a structured comparison framework that can be easily adapted to test additional operations.


## Getting Started


### Requirements

#### For Basic Execution Time Comparisons

- C++ compiler compatible with C++20 standard
- git
- cmake
- perf

#### For Detailed Profiling

- A Linux system with `perf` installed is required to perform the same profiling that is demonstrated in later sections.



### Building

#### Clone this Repository and Change Working Directory

```shell
https://github.com/duanegoodner/polymorphism-compare
cd polymorphism-compare
```

#### Create `build/` Directory
```shell
mkdir build
```

#### Generate Build System and Compile

```shell
cmake -B build
cmake --build build
```

#### CMake Configuration Options

The default compiler flags specified by `CMakeLists.txt` are `-O3 -march=native`. The following CMake confiuration options are also available.


| -D Option         | Compiler Flags |
|-------------------|---------------------------------|
| ENABLE_DEBUG      | `-O0 -g`                        |
| ENABLE_NO_INLINE  | `-O3 -march=native -fno-inline` |
| ENABLE_LOW_OPT    | `-O1 -march=native`             |
| ENABLE_PROFILING  | `-O3 -march=native -pg`         |
| RESET_DEFAULTS    | `-O3 -march=native`             |

For example, in the build command sequence shown above, if replace this:
```shell
cmake -B build
```
with this:
```shell
cmake -B build -DENABLE_PROFILING=ON
```
the compiler flags will be `-O3 -march=native -pg`.


### Running

#### Command Line Help

```shell
./build/bin/benchmark --help

# Output:
# Usage: ./build/bin/benchmark [polymorphism_category] [computation]
#  - No arguments: Runs all tests.
#  - With arguments: Runs a specific test.
#    Valid values for polymorphism_category: concepts crtp runtime 
#    Valid values for computation: expensive fma 
```

#### Run All Tests

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

#### Run a Single Test

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

### Profiling with `perf`


#### Build and Compile Profiling ON

```shell
cmake -B build -DENABLE_PROFILING=ON
cmake --build build
```







## Results

### Execution Time Comparison

| Polymorphism Type | Compute Function | Execution Time (s) |
|-------------------|-----------------|--------------------|
| Runtime          | FMA             | 1.50172           |
| Runtime          | Expensive       | 6.70873           |
| CRTP             | FMA             | 0.37878           |
| CRTP             | Expensive       | 0.38765           |
| Concepts         | FMA             | 0.37930           |
| Concepts         | Expensive       | 0.38852           |




### CPU Instructions and Branching Analysis

| Polymorphism Type | Compute Function | Instructions Retired (B) | Branches (B) | Branch Misses (K) |
|-------------------|-----------------|--------------------------|--------------|------------------|
| Runtime          | FMA             | 13.01                    | 3.00         | 26               |
| Runtime          | Expensive       | 154.02                   | 22.00        | 38               |
| CRTP             | FMA             | 4.00                     | 1.00         | 23               |
| CRTP             | Expensive       | 4.04                     | 1.01         | 24               |
| Concepts         | FMA             | 4.00                     | 1.00         | 23               |
| Concepts         | Expensive       | 4.00                     | 1.00         | 24               |

### Top-Down Performance Breakdown

| Polymorphism Type | Compute Function | Retiring (%) | Frontend Bound (%) | Backend Bound (%) |
|-------------------|-----------------|--------------|--------------------|-------------------|
| Runtime          | FMA             | 29.0         | 42.0               | 29.0              |
| Runtime          | Expensive       | 73.7         | 26.3               | 0.0               |
| CRTP             | FMA             | 25.1         | 1.6                | 73.3              |
| CRTP             | Expensive       | 25.1         | 2.0                | 72.9              |
| Concepts         | FMA             | 25.0         | 2.7                | 72.2              |
| Concepts         | Expensive       | 25.1         | 2.4                | 72.5              |









