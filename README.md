# polymorphism-compare
*Benchmarking Runtime vs Compile-Time Polymorphism for compute functions in C++*

## ⚡ TL;DR
- This project benchmarks Runtime Polymorphism vs. Compile-Time Polymorphism (CRTP & C++20 Concepts).
- Two compute functions are tested: one simple, one complex.
- CRTP and C++20 Concepts are significantly faster than Runtime Polymorphism, with the gap widening for more complex computations.
- CRTP and Concepts perform nearly identically, except Concepts show slightly higher frontend stalls and a minor (~2.5%) slowdown for the complex function.


## 📖 Background
When discussing *runtime vs compile-time polymorphism* in C++, it’s often stated that compile-time polymorphism can be faster due to factors like function inlining, elimination of virtual table lookups, and better compiler optimizations. However, these discussions are frequently followed by a caveat:

> *Performance differences are highly use-case dependent, and profiling is required to determine the impact in a given scenario.*

As I explored this, I expected to find many benchmark results from developers evaluating whether compile-time polymorphism would benefit their specific use cases.   However, after searching the web, GitHub, and academic literature, I realized that *hard numbers were surprisingly difficult to find* — most discussions acknowledge potential differences, but few provide *concrete performance data*.

## 🎯 What This Project Does


### 🔹 Compares Runtime vs Compile-Time Polymorphism Performance
This project benchmarks runtime vs compile-time polymorphism for two compute functions:

#### Simple Computation – Fused Multiply-Add (FMA):
```cpp
inline double ComputeFMA(double x) { return x * 1.414 + 2.718; }
```
#### Complex Computation – Sin, Log and a Square Root:
```cpp
inline double ComputeExpensive(double x) {
    return std::sin(x) * std::log(x + 1) + std::sqrt(x);
}
```
Each function is tested using:
- **Runtime polymorphism** via virtual function calls.
- **Compile-time polymorphism** using the Curiously Recurring Template Pattern (CRTP).
- **C++20 Concepts**  which enable compile-time type enforcement and selection, achieving behavior similar to polymorphism without inheritance.

### 🔹 Provides a Template for Benchmarking Other Compute Functions

This project is designed with modularity and ease of modification in mind, making it a useful starting point for benchmarking compile-time vs. runtime polymorphism with other compute functions. The codebase provides a structured comparison framework that can be easily adapted to test additional operations.


## 🚀 Getting Started

### 🔹 Requirements

#### For Basic Execution Time Comparisons

- C++ compiler compatible with C++20 standard
- git
- cmake

#### For Detailed Profiling (Optional)

- A Linux system with `perf` installed is required to perform the same profiling that is demonstrated in later sections.



### 🔹 Building

#### 🔹 Standard Build

Clone this repository and change working directory:

```shell
https://github.com/duanegoodner/polymorphism-compare
cd polymorphism-compare
```

Create the build directory:
```shell
mkdir build
```

Then generate the build system and compile:
```shell
cmake -B build
cmake --build build
```

#### 🔹CMake Configuration Options

The default compiler flags specified by `CMakeLists.txt` are `-O3 -march=native`. The following CMake confiuration options are also available.


| -D Option         | Compiler Flags |
|-------------------|-----------------------|
| `ENABLE_DEBUG`      | `-O0 -g`            |          |
| `ENABLE_NO_INLINE`  | `-fno-inline`       |
| `ENABLE_LOW_OPT`    | `-O1`               |
| `ENABLE_PROFILING`  | `-pg`               |
| `RESET_DEFAULTS`    | `-O3 -march=native` |

#### 🔹 Example: Enable Profiling
In the build command sequence, replacing this:
```shell
cmake -B build
```
with this:
```shell
cmake -B build -DENABLE_PROFILING=ON
```
will cause the compiler flags to be `-O3 -march=native -pg`.


### 🏃 Running the Benchmarks

#### 🔹 Command Line Help

```shell
./build/bin/benchmark --help
```
**Output:**
```
Usage: ./build/bin/benchmark [polymorphism_category] [computation] [-n iterations] [-s]
 - No arguments: Runs all tests with the default iteration count.
 - With two arguments: Runs a specific test with the default iteration count.
 - With '-n iterations': Runs all tests with a custom iteration count.
 - With '-s': Saves execution time data.

Valid arguments:
 ------------------------
 Polymorphism Categories:
 ------------------------
  - concepts
  - crtp
  - runtime

 Compute Functions:
 ------------------
  - expensive
  - fma

Other Options:
  --help              Show this help message
  -n [iterations]     Specify a custom iteration count
  -s                  Save execution time data
```


#### 🔹 Run All Tests

To run all possible combinations of polymorphism type and compute function, and save the execution time data, run: 
```shell
./build/bin/benchmark -s
```

**Output:**
```
Running: polymorphism_tests::TestRuntimeFMA
Iteration Count: 1000000000
FMA Computation: Runtime Polymorphism Time = 1.52441 seconds

Running: polymorphism_tests::TestRuntimeExpensive
Iteration Count: 1000000000
Expensive Computation: Runtime Polymorphism Time = 6.38158 seconds

Running: polymorphism_tests::TestCRTPFMA
Iteration Count: 1000000000
FMA Computation: CRTP Polymorphism Time = 0.378618 seconds

Running: polymorphism_tests::TestCRTPExpensive
Iteration Count: 1000000000
Expensive Computation: CRTP Polymorphism Time = 0.378513 seconds

Running: polymorphism_tests::TestConceptsFMA
Iteration Count: 1000000000
FMA Computation: C++20 Concepts Polymorphism Time = 0.378408 seconds

Running: polymorphism_tests::TestConceptsExpensive
Iteration Count: 1000000000
Expensive Computation: C++20 Concepts Polymorphism Time = 0.389357 seconds

Test results saved to: data/run_all_tests_results/2025-02-27-21-06-49-524.txt
```

#### 🔹 Run a Single Test

To run a single test, we need to provide arguments identifying the polymorphism type and the compute function:

For example:
```
./build/bin/benchmark crtp expensive
```
**Output:**
```
Running: polymorphism_tests::TestCRTPExpensive
Iteration Count: 1000000000
Expensive Computation: CRTP Polymorphism Time = 0.391168 seconds
```

#### 🔹 Custom Number of Iterations

The default number of times to run a compute function is 1e9. We can change this using the `-n` option:
```
./build/bin/benchmark runtime fma -n 500000
```
**Output:**
```
Running: polymorphism_tests::TestRuntimeFMA
Iteration Count: 500000
FMA Computation: Runtime Polymorphism Time = 0.00343413 seconds
```

## 🔎 Profiling with `perf`


### 🔹 Build with Profiling

```shell
cmake -B build -DENABLE_PROFILING=ON
cmake --build build
```

### 🔹 Run Profiling Tests

From the repo root, run:
```shell
./test/run_perf_tests.sh
```
To use a **custom number of iterations**:
```shell
./test/run_perf_tests.sh -n 80000000
```

### 🔹 Viewing Profiling Data

All profiling results will be saved in:
```
./data/perf/<timestamp-based-directory-name>/
```
Example:

```
All tests completed. Results are in: ./data/perf/2025-02-27_21-37-21/
```

### 📊 Results


#### ⏱ Execution Time Comparison


| Polymorphism Type | Compute Function | Execution Time (ms) | Error (ms) |
|-------------------|-----------------|--------------------|-----------|
| Runtime          | FMA             | 1510.22           | 2.95     |
| Runtime          | Expensive       | 6365.80           | 11.9     |
| CRTP             | FMA             | 378.86           | 0.392    |
| CRTP             | Expensive       | 378.16           | 0.626  |
| Concepts         | FMA             | 377.52           | 0.456  |
| Concepts         | Expensive       | 384.18           | 2.46   |
#### ✅ Observations:
- **Runtime Polymorphism (RP) is significantly slower:** ~4× slower than CRTP/Concepts for FMA and ~17× slower for Expensive computation.
- **CRTP and Concepts are closely matched**, with negligible performance differences between FMA and Expensive, except...
- **Concepts Expensive is slightly slower (~2.5%)** than other CRTP/Concepts tests, a small but statistically significant difference.
---

#### 🔄 CPU Instructions and Branching Analysis

| Polymorphism Type | Compute Function | Instructions Retired (x10<sup>9</sup>) | Branches (x10<sup>9</sup>) |
|-------------------|-----------------|--------------------------|--------------|
| Runtime          | FMA             | 13.01                    | 3.00         |
| Runtime          | Expensive       | 154.11                   | 22.02        | 
| CRTP             | FMA             | 4.00                     | 1.00         | 
| CRTP             | Expensive       | 4.00                     | 1.00         | 
| Concepts         | FMA             | 4.00                     | 1.00         | 
| Concepts         | Expensive       | 4.00                     | 1.00         | 

#### ✅ Observations:
- **CRTP & Concepts execute the same number of instructions** across both compute functions, suggesting aggressive compiler optimizations for Expensive.
- **RP executes significantly more instructions:**
    - **FMA:** ~4x more instructions and ~3x more branches
    - **Expensive:** ~40x more instructios and ~20x more branches

---

#### 🔬 Top-Down Performance Breakdown

| Polymorphism Type | Compute Function | Retiring (%) | Frontend Bound (%) | Backend Bound (%) |
|-------------------|-----------------|--------------|--------------------|-------------------|
| Runtime          | FMA             | 29.0         | 50.9               | 20.1              |
| Runtime          | Expensive       | 78.3         | 21.6               | 0.2               |
| CRTP             | FMA             | 25.1         | 2.1                | 72.8              |
| CRTP             | Expensive       | 25.1         | 1.2                | 73.7              |
| Concepts         | FMA             | 25.0         | 1.9                | 73.0              |
| Concepts         | Expensive       | 24.6         | 31.6               | 43.8              |

#### ✅ Observations:
- Runtime Polymorphism (RP) is less likely to stall, but this does not compensate for its higher instruction count.
- When RP instructions do stall, they are usually frontend bound (fetch latency issues).
- CRTP & Concepts are highly backend bound (~70%), meaning execution stalls are mostly due to data dependencies.
- Concepts Expensive shows increased frontend stalls (~31%), unlike CRTP Expensive.

---

### 🔑 Key Insights

#### 🏎 1️⃣ CRTP and Concepts are significantly faster than Runtime Polymorphism
- RP requires far more instructions and branches than CRTP/Concepts.
- Virtual function calls likely account for the large gap, with a much more pronounced slowdown in the Expensive function.
- RP instructions retire at a higher rate, but this does not compensate for the sheer number of extra instructions.
- When RP stalls, it's typically frontend bound, likely due to instruction fetch latency.

#### ⚖️ 2️⃣ CRTP and Concepts are *almost* identical
- CRTP and Concepts retire the same number of instructions & branches.
- Aggressive compiler optimizations likely explain why the Expensive computation is not heavier than FMA.
- Both are backend bound (~70%), meaning execution unit stalls dominate.
- Concepts Expensive is the only outlier, showing a higher frontend bound percentage (~31%) and a small but measurable slowdown.



