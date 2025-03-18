#include "benchmark_utils.hpp"
#include "test_runner.hpp"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

class BenchmarkUtilsTest : public ::testing::Test {
protected:
  std::string temp_dir;

  void SetUp() override {
    temp_dir = std::filesystem::temp_directory_path() / "test_output";
    std::filesystem::create_directories(temp_dir);
  }

  void TearDown() override {
    std::filesystem::remove_all(temp_dir); // Ensures cleanup
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

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST_F(BenchmarkUtilsTest, GenerateTimestampBasedFile_CreatesValidPath) {
    std::string generated_file = GenerateTimestampBasedFile(temp_dir);

    EXPECT_TRUE(generated_file.find(temp_dir) == 0);
    EXPECT_TRUE(generated_file.find(".txt") != std::string::npos);
    EXPECT_TRUE(std::filesystem::exists(temp_dir));
}

