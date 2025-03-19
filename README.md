# polymorphism-compare
*Benchmarking Runtime vs Compile-Time Polymorphism for compute functions in C++*

## ⚡ TL;DR
- This project benchmarks Runtime Polymorphism vs. Compile-Time Polymorphism (CRTP & C++20 Concepts).
- Two compute functions are tested: one simple, one complex.
- CRTP and C++20 Concepts are significantly faster than Runtime Polymorphism, with the gap widening for more complex computations.
- For the more complex computation, Runtime Polymorphism shows: 17x longer runtime, 27x more branches, 50x more instructions, and 5 orders of magnitude more memory accesses.
- CRTP and Concepts perform nearly identically.



## 📖 Background
When discussing *runtime vs compile-time polymorphism* in C++, it’s often stated that compile-time polymorphism can be faster due to factors like function inlining, elimination of virtual table lookups, and better compiler optimizations. However, these discussions are frequently followed by a caveat:

> *Performance differences are highly use-case dependent, and profiling is required to determine the impact in a given scenario.*

As I explored this, I expected to find many benchmark results from developers evaluating whether compile-time polymorphism would benefit their specific use cases.   However, after searching the web, GitHub, and academic literature, I realized that *hard numbers were surprisingly difficult to find* — most discussions acknowledge potential differences, but few provide *concrete performance data*.

## 🎯 What This Project Does


### 🔹 Examines how Permance is Affected by...

#### Approach to Polymorphism

- **Runtime polymorphism** via virtual function calls.
- **Compile-time polymorphism** using the Curiously Recurring Template Pattern (CRTP).
- **C++20 Concepts**  which enable compile-time type enforcement and selection, achieving behavior similar to polymorphism without inheritance.

#### Different Compute Functions

*Simple Computation – Fused Multiply-Add (FMA):*
```cpp
inline double ComputeFMA(double x) { return x * 1.414 + 2.718; }
```
*Complex Computation – Sin, Log and a Square Root:*
```cpp
inline double ComputeExpensive(double x) {
    return std::sin(x) * std::log(x + 1) + std::sqrt(x);
}
```

#### Compiler Optimization Levels

 - O0
 - O1
 - O2
 - O3


### 🔹 Provides a Template for Benchmarking Other Compute Functions

This project is designed with modularity and ease of modification in mind, making it a useful starting point for benchmarking compile-time vs. runtime polymorphism with other compute functions. The codebase provides a structured comparison framework that can be easily adapted to test additional operations.


## 🚀 Getting Started

### 🔹 Requirements

#### 🔹 For Basic Execution Time Comparisons

- C++ compiler compatible with C++20 standard
- git
- cmake
- Miniconda or Conda (optional, )

#### 🔹 For Detailed Profiling

- A Linux system with `perf` installed and a user with `sudo` privileges
- Conda or Miniconda (optional, but recommended for easy creation of Python environment for running automated tests)


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

#### 🔹 CMake Configuration Options

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

## 🧪 Testing Core Functionality

Before running benchmarks and profiling, we can verify the functionality of all core components using automated tests built with [GoogleTest](https://github.com/google/googletest). The test source code is located in `./test/core/`.  

To run all tests, use the provided script:

```bash
./test/core/run_tests.sh
```

## 🏃 Running the Benchmarks (without profiling)

The `benchmark` executable can be run directly from the command line.

### 🔹 Command Line Help

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

### 🔹 Benchmarking all Conditions
To run all possible combinations of polymorphism type and compute function, and save the execution time data, run:

```shell
./build/bin/benchmark -s
```
**Output:**
```shell
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
### 🔹 Run a Single Test
To run a single test, we need to provide arguments identifying the polymorphism type and the compute function. For example:

```shell
./build/bin/benchmark crtp expensive
```

**Output:**
```shell
Running: polymorphism_tests::TestCRTPExpensive
Iteration Count: 1000000000
Expensive Computation: CRTP Polymorphism Time = 0.391168 seconds
````

### 🔹 Custom Number of Iterations

```shell
./build/bin/benchmark runtime fma -n 500000
```
**Output:**
```shell
Running: polymorphism_tests::TestRuntimeFMA
Iteration Count: 500000
FMA Computation: Runtime Polymorphism Time = 0.00343413 seconds
```

## 🔎 Profiling with `perf`

We can use the Linux tool `perf` to gain more insight into differences among various forms of polymorphism and compute functions.


### 🔹 Automated Test Module

Python module `./test/profiling/multi_build_per_tester` can be used to run the benchmarks and save results as both raw `.txt` files and as cleaned `.feather` files that can be easily read into a Pandas DataFrame. All Python dependencies needed to run this module are specified in `environment.yml`.

#### Create and Activate Conda Environment
From the project root, run:
```shell
conda env create -f environment.yml
conda activate polymorphism-compare-env
````
Once our Conda environment is activated, can get command line help by running:

```shell
python test/profiling/multi_build_perf_tester.py --help
```
**Output:**
```
usage: multi_build_perf_tester.py [-h] [-o OPTIMIZATION_LEVELS [OPTIMIZATION_LEVELS ...]] [-p POLYMORPHISM_TYPES [POLYMORPHISM_TYPES ...]]
                                  [-c COMPUTE_FUNCTIONS [COMPUTE_FUNCTIONS ...]] [-r NUM_RUNS_PER_CONDITION] [-i NUM_ITERATIONS_PER_RUN]

Run performance tests for multiple build configurations.

options:
  -h, --help            show this help message and exit
  -o OPTIMIZATION_LEVELS [OPTIMIZATION_LEVELS ...], --optimization_levels OPTIMIZATION_LEVELS [OPTIMIZATION_LEVELS ...]
                        List of optimization levels to test (default = O0, O1, O2, O3).
  -p POLYMORPHISM_TYPES [POLYMORPHISM_TYPES ...], --polymorphism_types POLYMORPHISM_TYPES [POLYMORPHISM_TYPES ...]
                        List of polymorphism types to test (default = crtp, concepts, runtime).
  -c COMPUTE_FUNCTIONS [COMPUTE_FUNCTIONS ...], --compute_functions COMPUTE_FUNCTIONS [COMPUTE_FUNCTIONS ...]
                        List of compute functions to test (default = fma, expensive).
  -r NUM_RUNS_PER_CONDITION, --num_runs_per_condition NUM_RUNS_PER_CONDITION
                        Number of runs per condition (default = 5).
  -i NUM_ITERATIONS_PER_RUN, --num_iterations_per_run NUM_ITERATIONS_PER_RUN
                        Number of iterations per run (default = 1000000000).

```


### 🔹 Example: Profiling a Single Test Condition

To profile our FMA compute function using C++20 Concepts and aggressive (-O3) compiler optimization, we can run:

```
python -o 03 -p concepts -c fma
```
This will initiate a fresh build of the project binary (`./build/bin/benchmark`) prior to running the test. The terminal output should look similar to
```
...

✅ Build completed for -O3

Binary size for optimization levelO3: 79936 bytes (78.06 KB)

------------------------------------------------
Running perf for: Polymorphism Type = concepts
Compute Function = fma
Number of Runs = 5
Number of Iterations per Run = 1000000000
Running perf to collect detailed event data...
Results saved to: /home/duane/dproj/polymorphism-compare/data/perf/2025-03-19_15-37-38_O3/1_concepts_fma.txt
Re-running perf to collect summary data...
Results saved to: /home/duane/dproj/polymorphism-compare/data/perf/2025-03-19_15-37-38_O3/1_concepts_fma_summary.txt
Building Dataframe from detailed perf output...
DataFrame saved to /home/duane/dproj/polymorphism-compare/data/perf/2025-03-19_15-37-38_O3/perf_detailed_runs.feather
Building Dataframe from summary perf output...
DataFrame saved to /home/duane/dproj/polymorphism-compare/data/perf/2025-03-19_15-37-38_O3/perf_summary_runs.feather
```

Note that there are two sets of runs for each condition: one for detailed perf output and one for summary perf output (perf does not support collecting summary info and custom events in the same run).


### 🔹 Run All Tests

To run all possible combinations of polymorphism type and compute function, we can run the test module without passing any: 
```shell
python test/profiling/multi_build_perf_tester.py
```
This will result in builds at four different optimization levels, and will all possible combinations of Polymorphism Type and Compute Function with each resulting binary.


### 🔹 Profiling Data

Output from the profiling runs will be saved in a timestamped directory under `./data/perf/`. The directory  will include raw .txt data produced by `perf` as well as cleaned data in two `.feather` files: `perf_detailed_runs.feather` and `perf_summary_runs.feather`. Each of these can be imported into Python as a Pandas Dataframe. For example usage, see the code in`./test/profiling/view_dfs.py`.

> [!TIP]
> The compiler optimizaton level for a particular set of runs is indicated in the output directory name and is also included in `.txt` files located in the output directory.


#### 🔹 View Key Profiling Data

Markdown-formatted tables with some of the more interesting `perf` events data can be generated using Python module `test/profiling/view_dfs.py`. Get command line help for this simple module with:
```
python test/profiling/view_dfs.py --help
```
**Output**:
```
usage: view_dfs.py [-h] [-d DATA_DIR]

Analyze and display performance data.

options:
  -h, --help            show this help message and exit
  -d DATA_DIR, --data_dir DATA_DIR
                        Name of data-containing directory under ./data/perf/ (default =
                        view_dfs.DEFAULT_DATA_DIR_NAME)
```

> [!NOTE]
> Data in the **Results** section below were generated using the `view_dfs` module.

#### 🔹 Collecting Additional `perf` Events

The specific `perf` events collected by `./test/profiling/perf_test.py` are specified in `./tests/profiling/perf_events.json`. The content of this file can be customized to collect additional `perf` events.


## 📊 Running `perf` with Max (-O3) Compiler Optimization

This section describes test conditions and results of `perf` profiling performed with the most aggressive compiler optimizations (-O3).

### Experimental Setup

- CPU = 13th Gen Intel(R) Core(TM) i7-13700K
- OS = Debian 12.9
- Linux Kernel = 6.1.0-31-amd64
- Compiler = GCC 12.2.0

#### ⏱ Execution Time Comparison

| Polymorphism Type | Compute Function | Execution Time (ms) | Error (ms) |
|-------------------|-----------------|--------------------|-----------|
| Runtime          | FMA             | 1510.22           | 2.95     |
| Runtime          | Expensive       | 6365.80           | 11.9     |
| CRTP             | FMA             | 378.86           | 0.392    |
| CRTP             | Expensive       | 378.16           | 3.37  |
| Concepts         | FMA             | 377.52           | 0.456  |
| Concepts         | Expensive       | 378.40           | 3.56   |
#### ✅ Observations:
- **Runtime Polymorphism (RP) is significantly slower:** ~4× slower than CRTP/Concepts for FMA and ~17× slower for Expensive computation.
- **CRTP and Concepts are closely matched**, with negligible performance differences between FMA and Expensive
---

#### 📝 Instructions and Micro Ops

| Polymorphism Condition | Compute Function |   Mean Time |   Instructions | Micro Ops |
|:-----------------------|:-----------------|------------:|---------------:|----------:|
| Runtime               | FMA              |    5.8034   |      4.202e+10 | 5.903e+10 |
| Runtime               | Expensive        |   10.7571   |      1.82e+11  | 2.065e+11 |
| Concepts              | FMA              |    0.377829 |      3.998e+09 | 3.009e+09 |
| Concepts              | Expensive        |    0.3779   |      4.009e+09 | 3.017e+09 |
| CRTP                  | FMA              |    0.38061  |      4.012e+09 | 3.015e+09 |
| CRTP                  | Expensive        |    0.37642  |      4.003e+09 |  3.01e+09 |
#### ✅ Observations:

- **RP has significantly higher instruction and Micro Ops counts**: ~10x higher for FMA, and ~50x higher for Expensive.
- For **CRTP/Concepts, Micro Ops counts are lower than Instruction counts**, suggesting aggressive optimization by the compiler.

---


#### 🔀 Branching Analysis
| Polymorphism Condition | Compute Function |   Branches |   Branch Misses |   Miss Fraction |
|:-----------------------|:-----------------|-----------:|----------------:|----------------:|
| Runtime               | FMA              |  8.008e+09 |           13410 |       1.675e-06 |
| Runtime               | Expensive        |  2.699e+10 |           31860 |       1.18e-06  |
| Concepts              | FMA              |  1.002e+09 |             674 |       6.729e-07 |
| Concepts              | Expensive        |  1.005e+09 |            1752 |       1.743e-06 |
| CRTP                  | FMA              |  1.006e+09 |            1309 |       1.301e-06 |
| CRTP                  | Expensive        |  1.004e+09 |             966 |       9.618e-07 |

#### ✅ Observations:
- **Runtime Polymorphism (RP) has significantly higher branch counts** compared to CRTP/Concepts (8x higher for FMA, and 27x higher for Expensive).
- Branch prediction misses are low across all conditions.

---


#### 📥📤 Memory Access

| Polymorphism Condition | Compute Function | Stores    | Loads | Cache Misses | Cache Miss Fraction |
|:-----------------------|:-----------------|----------:|------------:|-------------:|--------------------:|
| Runtime               | FMA              |  1.30e+10 | 2.10e+10    | 8.41e+04     | 4.01e-06           |
| Runtime               | Expensive        |  2.10e+10 | 5.80e+10    | 9.40e+04     | 1.62e-06           |
| Concepts              | FMA              |  1.02e+05 | 1.97e+05    | 6.10e+04     | 3.10e-01           |
| Concepts              | Expensive        |  9.37e+04 | 1.79e+05    | 7.39e+04     | 4.13e-01           |
| CRTP                  | FMA              |  9.16e+04 | 1.73e+05    | 7.27e+04     | 4.21e-01           |
| CRTP                  | Expensive        |  9.40e+04 | 1.78e+05    | 7.25e+04     | 4.08e-01           |

#### ✅ Observations:
- **Runtime polymorphism: 5 orders of magnitude more stores and loads** than CRTP/Concepts.
- **Runtime Expensive** has ~40% more stores and ~2.8x more loads than Runtime FMA.
- **CRTP / Concepts have much higher cache miss rate** than RP, but still run much faster due to lower overall workload.

---


### 🔑 Key Insights

#### 🏎 1️⃣ CRTP and Concepts are significantly faster than Runtime Polymorphism
- RP requires more instructions & branches and massively higher memory access than CRTP/Concepts.
- Virtual function calls likely account for the large gap, with a much more pronounced slowdown in the Expensive function.
- The heap-to-stack usage ratio is likely higher for RP which would contribute to its higher memory access needs. 
- Uops : Instruction ratios clearly indicate more aggressive compiler optimization for CRTP/Concepts than RP. 


#### ⚖️ 2️⃣ CRTP and Concepts are identical
- CRTP and Concepts retire the same number of instructions & branches.
- Aggressive compiler optimizations likely explain why the Expensive computation is not heavier than FMA.


### Further Investigation

#### Assembly Code Review with `objdump`
- Assambly code review with `objdump` is underway.
- Code extracted as part of this analysis saved under `./test/objdump/`



