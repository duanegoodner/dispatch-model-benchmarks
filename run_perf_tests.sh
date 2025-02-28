#!/bin/bash

# Define the test parameters
POLYMORPHISM_TYPES=("runtime" "concepts" "crtp")
COMPUTE_FUNCTIONS=("fma" "expensive")

# Create a timestamped directory for results
TIMESTAMP=$(date +"%Y-%m-%d_%H-%M-%S")
OUTPUT_DIR="./data/perf/${TIMESTAMP}/"
mkdir -p "$OUTPUT_DIR"

echo "Saving perf results to: $OUTPUT_DIR"

# Loop over all polymorphism types and compute functions
for POLY_TYPE in "${POLYMORPHISM_TYPES[@]}"; do
    for COMP_FUNC in "${COMPUTE_FUNCTIONS[@]}"; do
        OUTPUT_FILE="${OUTPUT_DIR}${POLY_TYPE}_${COMP_FUNC}_perf_summary.txt"
        echo "Running perf for: ${POLY_TYPE} - ${COMP_FUNC}"
        
        # Run perf, capture terminal output (including iteration count), and save results
        sudo perf stat -r 5 ./build/bin/benchmark "$POLY_TYPE" "$COMP_FUNC" 2>&1 | tee "$OUTPUT_FILE"

        echo "Results saved to: $OUTPUT_FILE"
        echo "-------------------------------------------------"
    done
done

echo "All tests completed. Results are in: $OUTPUT_DIR"
