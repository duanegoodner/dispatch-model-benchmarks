#include <gtest/gtest.h>
#include "cli_utils.hpp"

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

TEST_F(CLIUtilsTest, ParseIterationCount) {
  int argc = 3;
  const char *argv_valid[] = {"program", "-n", "100"};
  int remainig_argc = argc;
  auto valid_result =
      ParseIterationCount(argc, const_cast<char **>(argv_valid), remainig_argc);
  EXPECT_EQ(valid_result, 100);

  const char *argv_invalid[] = {"program", "-n", "abc"};
  auto invalid_result = ParseIterationCount(
      argc,
      const_cast<char **>(argv_invalid),
      remainig_argc
  );
  EXPECT_FALSE(invalid_result.has_value());
}

TEST_F(CLIUtilsTest, IsValidPolymorphismCategory) {
  const std::string valid_category{"concepts"};
  auto valid_result = IsValidPolymorphismCategory(valid_category);
  EXPECT_TRUE(valid_result);

  const std::string invalid_category("not_a_category");
  auto invalid_result = IsValidPolymorphismCategory(invalid_category);
  EXPECT_FALSE(invalid_result);
}

TEST_F(CLIUtilsTest, IsValidCompuataton) {
  const std::string valid_category{"concepts"};
  const std::string valid_computation{"fma"};
  const std::string invalid_category{"not_a_category"};
  const std::string invalid_computation{"not_a_computation"};

  auto valid_result = IsValidComputation(valid_category, valid_computation);
  EXPECT_TRUE(valid_result);

  auto invalid_result =
      IsValidComputation(invalid_category, invalid_computation);
  EXPECT_FALSE(invalid_result);
}

TEST_F(CLIUtilsTest, ParseAndValidateArguments) {
  int argc = 3;
  const char *argv_valid[] = {"program", "-n", "100"};
  int remainig_argc = argc;

  auto valid_result = ParseAndValidateArguments(
      argc,
      const_cast<char **>(argv_valid),
      remainig_argc
  );

  EXPECT_EQ(valid_result, 100);

  const char *argv_iterations_not_specified[] = {"program"};
  int argc_iterations_unspecified = 1;
  int remaining_argc_interations_unspecified = argc_iterations_unspecified;
  auto default_result = ParseAndValidateArguments(
      argc_iterations_unspecified,
      const_cast<char **>(argv_iterations_not_specified),
      remaining_argc_interations_unspecified
  );
  EXPECT_EQ(default_result, 1000000000);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}