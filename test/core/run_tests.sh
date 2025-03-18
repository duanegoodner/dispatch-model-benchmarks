#!/bin/bash

# Adjusted test directory path
TEST_DIR="./build/bin/test"

# Check if the directory exists
if [ ! -d "$TEST_DIR" ]; then
    echo "Error: Test directory '$TEST_DIR' not found. Did you run cmake and build the project?"
    exit 1
fi

# Find all test executables and run them
for test_exe in "$TEST_DIR"/*; do
    if [[ -x "$test_exe" && ! -d "$test_exe" ]]; then  # Ensure it's an executable file
        echo "Running $test_exe..."
        "$test_exe"
        echo "--------------------------------------------"
    fi
done

echo "All tests completed."
