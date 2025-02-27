#include "benchmark_utils.hpp"
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <string>
// #include <ctime>


// Prevent compiler optimizations by using a volatile variable
volatile double prevent_optimization = 0.0;

void PrintTime(
    const std::string &label,
    std::chrono::duration<double> elapsed
) {
  std::cout << label << " Time = " << elapsed.count() << " seconds" << std::endl
            << std::endl;
}

std::string GenerateTimestampBasedFile(std::string output_dir) {
  std::filesystem::create_directories(output_dir);

  // Generate timestamp-based filename
  auto now = std::chrono::system_clock::now();
  auto now_time = std::chrono::system_clock::to_time_t(now);
  auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                    now.time_since_epoch()
                ) %
                1000;

  std::ostringstream filename;
  filename << output_dir
           << std::put_time(std::localtime(&now_time), "%Y-%m-%d-%H-%M-%S")
           << "-" << now_ms.count() << ".txt";

  return filename.str();
}
