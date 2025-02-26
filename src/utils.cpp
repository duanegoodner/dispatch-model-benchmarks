#include "utils.hpp"

// Prevent compiler optimizations by using a volatile variable
volatile double prevent_optimization = 0.0;

void PrintTime(
    const std::string &label,
    std::chrono::duration<double> elapsed
) {
  std::cout << label << " Time: " << elapsed.count() << " seconds\n";
}
