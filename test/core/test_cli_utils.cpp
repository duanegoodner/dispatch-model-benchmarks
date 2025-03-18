#include "cli_utils.hpp"
#include <gtest/gtest.h>

class CLIUtilsTest : public ::testing::Test {};

TEST_F(CLIUtilsTest, PrintUsage) {

  const char *program_name = "My Program";
  PrintUsage(program_name);
}

TEST_F(CLIUtilsTest, GetTestCaseMap) {
    auto test_case_map = test_runner::GetTestCaseMap();
}

TEST_F(CLIUtilsTest, ParseSaveExecutionTimesFlag) {
    int argc = 1;
    char **argv{};
    int remaining_argc = 0;

    auto result = ParseSaveExecutionTimesFlag(argc, argv, remaining_argc);
    EXPECT_FALSE(result);
}

TEST_F(CLIUtilsTest, HandleHelpOption) {
    int argc = 2;
    const char *argv[] = {"executable/path", "--help"};
    auto result = HandleHelpOption(argc, const_cast<char **>(argv));

    EXPECT_TRUE(result);
}

// TEST_F(CLIUtilsTest, ParseIterationCount) {
//     int argc = 3;
//     char *argv_valid[] =  { "program", "-n", "100" };
//     int remainig_argc = argc;
//     auto valid_result = ParseIterationCount(argc, argv_valid, remainig_argc);
//     EXPECT_EQ(valid_result, 100);

//     char *argv_invalid[] = {"program", "-n", "abc"};
//     auto invalid_result = ParseIterationCount(argc, argv_invalid, remainig_argc)
// }

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}