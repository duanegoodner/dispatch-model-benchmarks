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
  std::cout << label << " Time = " << elapsed.count() << " seconds"
            << std::endl << std::endl;
}

std::chrono::duration<double> RunTestCase(
    const TestCase &test_case,
    size_t iterations
) {
  std::cout << "Running: " << test_case.name << std::endl;
  std::cout << "Iteration Count: " << iterations << std::endl;

  auto start = std::chrono::high_resolution_clock::now();
  test_case.function(iterations);
  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> elapsed_time = end - start;
  return elapsed_time;
}

std::string GenerateTimestampBasedFile(std::string output_dir) {

  // Ensure output directory exists
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

int ValidateOutfileStream(std::ofstream &outfile, const std::string &filepath) {
  if (!outfile) {
    std::cerr << "Error: Unable to open file for writing: " << filepath
              << std::endl;
    return EXIT_FAILURE;
  }
  return 0;
}

void WriteCompileFlagsInfo(std::ofstream &outfile) {
  outfile << "Compiler Flags: " << COMPILER_FLAGS << "\n\n";
}

void WriteNumberOfIterations(size_t num_iterations, std::ofstream &outfile) {
  outfile << "Number of Iterations: " << num_iterations << "\n\n";
}

void WriteMarkdownTableHeader(std::ofstream &outfile) {
  outfile << "| Polymorphism Type | Compute Function | Time (seconds) |\n";
  outfile << "|-------------------|-----------------|---------------|\n";
}

void WriteMarkdownTableRow(
    std::ofstream &outfile,
    const std::string &polymorphism_category,
    const std::string &computation_label,
    std::chrono::duration<double> elapsed_time
) {
  outfile << "| " << polymorphism_category << " | " << computation_label
          << " | " << elapsed_time.count() << " |\n";
}

void WriteSingleTestResultToFile(
    const std::string &output_dir,
    size_t iterations,
    const std::string &polymorphism_category,
    const std::string &computation_label,
    std::chrono::duration<double> elapsed_time
) {
  auto filepath = GenerateTimestampBasedFile(output_dir);
  std::ofstream outfile(filepath);
  ValidateOutfileStream(outfile, filepath);

  // Write test details
  WriteCompileFlagsInfo(outfile);
  WriteNumberOfIterations(iterations, outfile);
  WriteMarkdownTableHeader(outfile);
  WriteMarkdownTableRow(
      outfile,
      polymorphism_category,
      computation_label,
      elapsed_time
  );

  std::cout << "Single test result saved to: " << filepath << std::endl
            << std::endl;
}
