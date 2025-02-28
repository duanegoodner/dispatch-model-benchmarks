#include "cli_utils.hpp"
#include "test_runner.hpp"
#include <cstdlib>
#include <iostream>
#include <string_view>

// Default number of iterations
constexpr size_t kDefaultNumIterations = 1'000'000'000;

int main(int argc, char **argv) {
  return RunFromCLI(argc, argv);
}
