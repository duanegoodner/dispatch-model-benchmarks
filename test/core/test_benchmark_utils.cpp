#include "benchmark_utils.hpp"
#include "test_runner.hpp"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <gtest/gtest.h>

class BenchmarkUtilsTest : public ::testing::Test {
protected:
  std::string temp_dir;
  std::string valid_file;
  std::string invalid_file;
  std::string temp_file;

  void SetUp() override {
    temp_dir = std::filesystem::temp_directory_path() / "test_output";
    std::filesystem::create_directories(temp_dir);

    valid_file = temp_dir + "/valid_test.txt";
    invalid_file = "/root/invalid_test.txt"; // Likely an unwritable location
    temp_file = temp_dir + "/test_output.txt"; // Used for new tests
  }

  void TearDown() override {
    std::filesystem::remove_all(temp_dir); // Cleanup after test
  }

  // Helper function to read file contents for validation
  std::string ReadFileContents(const std::string &filepath) {
    std::ifstream file(filepath);
    std::ostringstream content;
    content << file.rdbuf();
    return content.str();
  }
};

TEST_F(BenchmarkUtilsTest, PrintTime) {
  auto start = std::chrono::high_resolution_clock::now();
  auto end = std::chrono::high_resolution_clock::now();
  auto elapsed = end - start;
  const std::string label{"my_label"};

  PrintTime(label, elapsed);
}

TEST_F(BenchmarkUtilsTest, RunTestCase) {
  const std::string polymorphism_category = "crtp";
  const std::string computation_label = "fma";
  auto test_case =
      test_runner::GetSingleTestCase(polymorphism_category, computation_label);
  auto elapsed_time = RunTestCase(test_case, 1000000000);
  EXPECT_GT(elapsed_time.count(), 0.0);
}

TEST_F(BenchmarkUtilsTest, GenerateTimestampBasedFile_CreatesValidPath) {
  std::string generated_file = GenerateTimestampBasedFile(temp_dir);

  EXPECT_TRUE(generated_file.find(temp_dir) == 0);
  EXPECT_TRUE(generated_file.find(".txt") != std::string::npos);
  EXPECT_TRUE(std::filesystem::exists(temp_dir));
}

TEST_F(BenchmarkUtilsTest, ValidateOutfileStream_Success) {
  std::ofstream outfile(valid_file);
  ASSERT_TRUE(outfile.is_open()); // Ensure file actually opened

  int result = ValidateOutfileStream(outfile, valid_file);

  EXPECT_EQ(result, 0); // Expected success
}

TEST_F(BenchmarkUtilsTest, ValidateOutfileStream_Failure) {
  std::ofstream outfile(invalid_file); // Attempt to open a protected path
  ASSERT_FALSE(outfile.is_open());     // Ensure it failed to open

  int result = ValidateOutfileStream(outfile, invalid_file);

  EXPECT_EQ(result, EXIT_FAILURE); // Expected failure
}

TEST_F(BenchmarkUtilsTest, WriteCompileFlagsInfo_WritesCorrectly) {
  std::ofstream outfile(temp_file);
  WriteCompileFlagsInfo(outfile);
  outfile.close();

  std::string content = ReadFileContents(temp_file);
  EXPECT_TRUE(content.find("Compiler Flags:") != std::string::npos);
}

TEST_F(BenchmarkUtilsTest, WriteNumberOfIterations_WritesCorrectly) {
  std::ofstream outfile(temp_file);
  WriteNumberOfIterations(500, outfile);
  outfile.close();

  std::string content = ReadFileContents(temp_file);
  EXPECT_TRUE(content.find("Number of Iterations: 500") != std::string::npos);
}

TEST_F(BenchmarkUtilsTest, WriteMarkdownTableHeader_WritesCorrectly) {
  std::ofstream outfile(temp_file);
  WriteMarkdownTableHeader(outfile);
  outfile.close();

  std::string content = ReadFileContents(temp_file);
  EXPECT_TRUE(content.find("| Polymorphism Type | Compute Function | Time (seconds) |") != std::string::npos);
}

TEST_F(BenchmarkUtilsTest, WriteMarkdownTableRow_WritesCorrectly) {
  std::ofstream outfile(temp_file);
  WriteMarkdownTableRow(outfile, "crtp", "fma", std::chrono::duration<double>(1.23));
  outfile.close();

  std::string content = ReadFileContents(temp_file);
  EXPECT_TRUE(content.find("| crtp | fma | 1.23 |") != std::string::npos);
}

TEST_F(BenchmarkUtilsTest, WriteSingleTestResultToFile_CreatesValidOutput) {
    // Capture the file path from the function itself
    std::string filepath = GenerateTimestampBasedFile(temp_dir);
    WriteSingleTestResultToFile(temp_dir, 1000, "runtime", "expensive", std::chrono::duration<double>(0.456));

    ASSERT_TRUE(std::filesystem::exists(filepath)) << "Test output file should exist";
    std::string content = ReadFileContents(filepath);

    EXPECT_TRUE(content.find("Compiler Flags:") != std::string::npos);
    EXPECT_TRUE(content.find("Number of Iterations: 1000") != std::string::npos);
    EXPECT_TRUE(content.find("| runtime | expensive | 0.456 |") != std::string::npos);
}
 

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
