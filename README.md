# polymorphism-compare
*Benchmarking Runtime vs Compile-Time Polymorphism for compute functions in C++*

## ⚡ TL;DR
- This project benchmarks Runtime Polymorphism vs. Compile-Time Polymorphism (CRTP & C++20 Concepts) across multiple compiler optimization levels (-O0, -O1, -O2, -O3).
- Without compiler optimizations (-O0), Runtime Polymorphism is the fastest due to lower template instantiation overhead.
- However, CRTP and Concepts benefit massively from compiler optimizations, while Runtime Polymorphism sees only minor improvements—especially for complex computations.
- With full optimization (-O3), Runtime Polymorphism has:
    - 17× longer execution time
    - 27× more branches
    - 50× more instructions
    - 5 orders of magnitude more memory accesses
- C++20 Concepts perform similarly to CRTP at -O3, but miss out on CRTP’s -O2 performance gains.

> [!IMPORTANT]
> **Polymorphism performance is highly use-case dependent.**  While these benchmarks highlight significant trends, **your mileage may vary** depending on factors like CPU architecture, compiler version, optimization settings, and workload characteristics.  Always profile your own use case before making big design decisions!


## 📖 Background
When discussing *runtime vs compile-time polymorphism* in C++, it’s often stated that compile-time polymorphism can be faster due to factors like function inlining, elimination of virtual table lookups, and better compiler optimizations. However, these discussions are frequently followed by a caveat:

> *Performance differences are highly use-case dependent, and profiling is required to determine the impact in a given scenario.*

As I explored this, I expected to find many benchmark results from developers evaluating whether compile-time polymorphism would benefit their specific use cases.   However, after searching the web, GitHub, and academic literature, I realized that *hard numbers were surprisingly difficult to find* — most discussions acknowledge potential differences, but few provide *concrete performance data*.

## 🎯 What This Project Does


### 🔹 Examines how Permance is Affected by...

#### Approach to Polymorphism

- **Runtime polymorphism** via virtual function calls.
- **Compile-time polymorphism** using the Curiously Recurring Template Pattern (CRTP).
- **C++20 Concepts**  which enable compile-time type enforcement and selection, achieving behavior similar to polymorphism without inheritance &mdash; and offer much clearer compiler error messages than other uses of template programming (including CRTP).

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
Running: polymorphism_tests::TestRuntime FMA
Iteration Count: 1000000000
FMA Computation: Runtime Polymorphism Time = 1.52441 seconds

Running: polymorphism_tests::TestRuntime Expensive
Iteration Count: 1000000000
Expensive Computation: Runtime Polymorphism Time = 6.38158 seconds

Running: polymorphism_tests::TestCRTP FMA
Iteration Count: 1000000000
FMA Computation: CRTP Polymorphism Time = 0.378618 seconds

Running: polymorphism_tests::TestCRTP Expensive
Iteration Count: 1000000000
Expensive Computation: CRTP Polymorphism Time = 0.378513 seconds

Running: polymorphism_tests::TestConcepts FMA
Iteration Count: 1000000000
FMA Computation: C++20 Concepts Polymorphism Time = 0.378408 seconds

Running: polymorphism_tests::TestConcepts Expensive
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
Running: polymorphism_tests::TestCRTP Expensive
Iteration Count: 1000000000
Expensive Computation: CRTP Polymorphism Time = 0.391168 seconds
````

### 🔹 Custom Number of Iterations

```shell
./build/bin/benchmark runtime fma -n 500000
```
**Output:**
```shell
Running: polymorphism_tests::TestRuntime FMA
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


## 🏃 Running `perf` for All Test Conditions

This section describes test conditions and results of `perf` profiling performed for all possible combinations:

- Polymorphism Type: Runtime, CRTP, C++20 Concepts
- Compute Function: FMA, Expensive
- Compiler Optimization Levels: -O0, -O1, -O2, -O3

### Test Platform Details

- CPU = 13th Gen Intel(R) Core(TM) i7-13700K
- OS = Debian 12.9
- Linux Kernel = 6.1.0-31-amd64
- Compiler = GCC 12.2.0


### 🔑 Key Insights

> [!NOTE]
> The data in the results section below is from **fresh test runs** including all optimization levels (`-O0`, `-O1`, `-O2`, `-O3`).  
> A **detailed analysis and visualization of trends will be added soon.**

- **Runtime Polymorphism (`virtual` functions) is consistently the slowest** across all optimization levels, with the gap widening significantly for complex computations.
- **CRTP is consistently the fastest approach**, outperforming both Runtime Polymorphism and C++20 Concepts, particularly at `-O2`.
- **C++20 Concepts behave similarly to CRTP at `-O3` but struggle at `-O2`**, showing significantly slower execution times.
- **Compiler Optimization Level Matters**:
  - **At `-O1` and `-O3`**, Concepts and CRTP perform nearly identically.
  - **At `-O2`**, CRTP remains fast, but Concepts slow down dramatically.
- **Branch Mispredictions & Cache Misses** are significantly worse for Runtime Polymorphism, making it less efficient than compile-time alternatives.
- **Binary size shrinks from `-O0` to `-O3`**, with **`-O3` generating the smallest executables** while also delivering the best performance.
- **Loop Unrolling and Function Inlining seem to play a major role in CRTP’s advantage**, particularly at `-O2` where Concepts do not benefit from the same optimizations.


### 📌 Next Steps
- Investigate **assembly differences (`objdump`)** between Concepts and CRTP at `-O2`.  
- Examine **additional compiler flags** to see if Concepts can be optimized further.  
- Compare **binary size trends** for all approaches across optimization levels.  


## 📊 Event Data from `perf`


### ⏱ 📝 Execution Time and Instructions

#### 🐌 Optimization Level: -O0

| Test Name         |   Time (s) |      Branches |   Instructions |   Micro Ops |
|:------------------|------------:|--------------:|---------------:|------------------:|
| Runtime FMA        |     2.95447 |   6.98418e+09 |    5.48726e+10 |       6.23562e+10 |
| Runtime Expensive  |    10.0609  |   3.00041e+10 |    2.021e+11   |       2.19918e+11 |
| Concepts FMA       |     3.33434 |   7.00953e+09 |    5.10138e+10 |       5.94976e+10 |
| Concepts Expensive |     9.9803  |   2.99839e+10 |    1.97852e+11 |       2.14807e+11 |
| CRTP FMA           |     4.5475  |   9.00312e+09 |    6.50036e+10 |       7.57248e+10 |
| CRTP Expensive     |    11.8889  |   3.19839e+10 |    2.11878e+11 |       2.31614e+11 |

#### 🐢 Optimization Level: -O1

| Test Name         |   Time (s) |      Branches |   Instructions |   Micro Ops |
|:------------------|------------:|--------------:|---------------:|------------------:|
| Runtime FMA        |    1.51398  |   4.00473e+09 |    1.30049e+10 |       1.40098e+10 |
| Runtime Expensive  |    6.7236   |   2.29037e+10 |    1.57141e+11 |       1.62151e+11 |
| Concepts FMA       |    0.37788  |   1.00527e+09 |    4.00984e+09 |       3.01393e+09 |
| Concepts Expensive |    6.17581  |   2.20039e+10 |    1.55921e+11 |       1.59944e+11 |
| CRTP FMA           |    0.38335  |   1.00169e+09 |    3.99376e+09 |       3.00774e+09 |
| CRTP Expensive     |    0.379375 |   1.00166e+09 |    3.99497e+09 |       3.00839e+09 |


#### 🚗 Optimization Level: -O2

| Test Name         |   Time (s) |      Branches |   Instructions |   Micro Ops |
|:------------------|------------:|--------------:|---------------:|------------------:|
| Runtime FMA        |    1.51137  |   3.00754e+09 |    1.30168e+10 |       1.40358e+10 |
| Runtime Expensive  |    6.4254   |   2.19883e+10 |    1.5375e+11  |       1.58809e+11 |
| Concepts FMA       |    1.51909  |   3.00082e+09 |    1.20043e+10 |       1.20152e+10 |
| Concepts Expensive |    6.4359   |   2.20507e+10 |    1.53393e+11 |       1.57767e+11 |
| CRTP FMA           |    0.38022  |   1.00184e+09 |    3.99637e+09 |       3.00877e+09 |
| CRTP Expensive     |    0.378914 |   9.9946e+08  |    3.99163e+09 |       3.00997e+09 |


#### 🚀 Optimization Level: -O3

| Test Name         |   Time (s) |      Branches |   Instructions |   Micro Ops |
|:------------------|------------:|--------------:|---------------:|------------------:|
| Runtime FMA        |    1.51615  |   3.00083e+09 |    1.30127e+10 |       1.4032e+10  |
| Runtime Expensive  |    6.4408   |   2.19913e+10 |    1.53851e+11 |       1.58795e+11 |
| Concepts FMA       |    0.378842 |   1.00299e+09 |    3.99507e+09 |       3.00684e+09 |
| Concepts Expensive |    0.379948 |   1.00221e+09 |    3.99772e+09 |       3.00826e+09 |
| CRTP FMA           |    0.38215  |   1.00139e+09 |    3.99694e+09 |       3.01604e+09 |
| CRTP Expensive     |    0.38097  |   1.00037e+09 |    3.99236e+09 |       3.00623e+09 |

---

### 🔀 Branches

#### 🐌 Optimization Level: -O0

| Test Name         |      Branches |    Branch Misses |   Miss Fraction |
|:------------------|--------------:|-----------------:|----------------:|
| Runtime FMA        |   6.98418e+09 |   7656           |     1.09619e-06 |
| Runtime Expensive  |   3.00041e+10 |  32902           |     1.09658e-06 |
| Concepts FMA       |   7.00953e+09 |      4.38924e+06 |     0.000626182 |
| Concepts Expensive |   2.99839e+10 |  29983           |     9.9997e-07  |
| CRTP FMA           |   9.00312e+09 | 175674           |     1.95126e-05 |
| CRTP Expensive     |   3.19839e+10 |  47488           |     1.48475e-06 |

#### 🐢 Optimization Level: -O1

| Test Name         |      Branches |   Branch Misses |   Miss Fraction |
|:------------------|--------------:|----------------:|----------------:|
| Runtime FMA        |   4.00473e+09 |            5640 |     1.40834e-06 |
| Runtime Expensive  |   2.29037e+10 |           21518 |     9.39501e-07 |
| Concepts FMA       |   1.00527e+09 |             956 |     9.50986e-07 |
| Concepts Expensive |   2.20039e+10 |           25431 |     1.15575e-06 |
| CRTP FMA           |   1.00169e+09 |            1866 |     1.86286e-06 |
| CRTP Expensive     |   1.00166e+09 |            2317 |     2.31317e-06 |

#### 🚗 Optimization Level: -O2

| Test Name         |      Branches |   Branch Misses |   Miss Fraction |
|:------------------|--------------:|----------------:|----------------:|
| Runtime FMA        |   3.00754e+09 |            6469 |     2.15093e-06 |
| Runtime Expensive  |   2.19883e+10 |           24512 |     1.11477e-06 |
| Concepts FMA       |   3.00082e+09 |            3955 |     1.31797e-06 |
| Concepts Expensive |   2.20507e+10 |           18777 |     8.51538e-07 |
| CRTP FMA           |   1.00184e+09 |            1249 |     1.24671e-06 |
| CRTP Expensive     |   9.9946e+08  |             956 |     9.56516e-07 |


#### 🚀 Optimization Level: -O3

| Test Name         |      Branches |   Branch Misses |   Miss Fraction |
|:------------------|--------------:|----------------:|----------------:|
| Runtime FMA        |   3.00083e+09 |            6426 |     2.1414e-06  |
| Runtime Expensive  |   2.19913e+10 |           28481 |     1.2951e-06  |
| Concepts FMA       |   1.00299e+09 |            2031 |     2.02495e-06 |
| Concepts Expensive |   1.00221e+09 |            3443 |     3.43542e-06 |
| CRTP FMA           |   1.00139e+09 |            1766 |     1.76355e-06 |
| CRTP Expensive     |   1.00037e+09 |            1886 |     1.88531e-06 |

---

### 📥📤 Memory Access

#### 🐌 Optimization Level: -O0

| Test Name         |   mem_inst_retired.all_stores |   mem_inst_retired.all_loads |   cache-misses |   cache-Miss Fraction |
|:------------------|------------------------------:|-----------------------------:|---------------:|----------------------:|
| Runtime FMA        |                   1.30384e+10 |                  2.30473e+10 |          75973 |           3.2964e-06  |
| Runtime Expensive  |                   2.19943e+10 |                  6.29821e+10 |         186677 |           2.96397e-06 |
| Concepts FMA       |                   1.30167e+10 |                  1.90293e+10 |          81142 |           4.26405e-06 |
| Concepts Expensive |                   2.20129e+10 |                  5.90384e+10 |         121468 |           2.05744e-06 |
| CRTP FMA           |                   1.70017e+10 |                  2.30038e+10 |         103357 |           4.49304e-06 |
| CRTP Expensive     |                   2.60163e+10 |                  6.30247e+10 |         126241 |           2.00304e-06 |

#### 🐢 Optimization Level: -O1

| Test Name         |   mem_inst_retired.all_stores |   mem_inst_retired.all_loads |   cache-misses |   cache-Miss Fraction |
|:------------------|------------------------------:|-----------------------------:|---------------:|----------------------:|
| Runtime FMA        |                   2.00067e+09 |                  7.00035e+09 |         120655 |           1.72356e-05 |
| Runtime Expensive  |                   1.10183e+10 |                  4.51358e+10 |         136076 |           3.01481e-06 |
| Concepts FMA       |              113367           |             207912           |         104863 |           0.504362    |
| Concepts Expensive |                   1.09808e+10 |                  4.28297e+10 |         149766 |           3.49678e-06 |
| CRTP FMA           |              103279           |             178090           |         100107 |           0.562115    |
| CRTP Expensive     |               98230           |             181020           |         101927 |           0.56307     |

#### 🚗 Optimization Level: -O2

| Test Name         |   mem_inst_retired.all_stores |   mem_inst_retired.all_loads |   cache-misses |   cache-Miss Fraction |
|:------------------|------------------------------:|-----------------------------:|---------------:|----------------------:|
| Runtime FMA        |                   2.00467e+09 |                  7.01407e+09 |         126230 |           1.79967e-05 |
| Runtime Expensive  |                   1.00014e+10 |                  4.40329e+10 |         100436 |           2.28093e-06 |
| Concepts FMA       |                   2.00108e+09 |                  5.00331e+09 |         104256 |           2.08374e-05 |
| Concepts Expensive |                   9.91248e+09 |                  4.16685e+10 |         120469 |           2.89113e-06 |
| CRTP FMA           |                        116233 |             212866           |          99111 |           0.465603    |
| CRTP Expensive     |                        105460 |             202577           |          89321 |           0.440924    |

#### 🚀 Optimization Level: -O3

| Test Name         |   mem_inst_retired.all_stores |   mem_inst_retired.all_loads |   cache-misses |   cache-Miss Fraction |
|:------------------|------------------------------:|-----------------------------:|---------------:|----------------------:|
| Runtime FMA        |                   2.00139e+09 |                  7.00331e+09 |         134751 |           1.9241e-05  |
| Runtime Expensive  |                   9.97602e+09 |                  4.39428e+10 |         129370 |           2.94406e-06 |
| Concepts FMA       |                         96455 |             180536           |         124091 |           0.687348    |
| Concepts Expensive |                         94656 |             181633           |         131838 |           0.725848    |
| CRTP FMA           |                        109071 |             215144           |         112663 |           0.523663    |
| CRTP Expensive     |                        107460 |             201802           |         118616 |           0.587784    |

---
