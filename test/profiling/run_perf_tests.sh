#!/bin/bash

# Define the test parameters
POLYMORPHISM_TYPES=("runtime" "concepts" "crtp")
COMPUTE_FUNCTIONS=("fma" "expensive")

# Default to not specifying iterations (let the program use its default)
ITERATIONS=""

# Parse optional -n argument for custom iteration count
while [[ $# -gt 0 ]]; do
    case $1 in
        -n)
            if [[ -n $2 && $2 =~ ^[0-9]+$ ]]; then
                ITERATIONS="-n $2"
                shift 2
            else
                echo "Error: Invalid or missing value for -n option. Please provide a positive integer."
                exit 1
            fi
            ;;
        *)
            echo "Error: Unknown argument '$1'"
            exit 1
            ;;
    esac
done

# Create a timestamped directory for results
TIMESTAMP=$(date +"%Y-%m-%d_%H-%M-%S")
OUTPUT_DIR="./data/perf/${TIMESTAMP}/"
mkdir -p "$OUTPUT_DIR"

echo "Results will be saved in directory: $OUTPUT_DIR"

# Loop over all polymorphism types and compute functions
for POLY_TYPE in "${POLYMORPHISM_TYPES[@]}"; do
    for COMP_FUNC in "${COMPUTE_FUNCTIONS[@]}"; do
        OUTPUT_FILE="${OUTPUT_DIR}${POLY_TYPE}_${COMP_FUNC}_perf_summary.txt"
        echo
        echo "-------------------------------------------------"
        echo "Running perf for: Polymorphism Type = ${POLY_TYPE}, Compute Function = ${COMP_FUNC}"
        [[ -n "$ITERATIONS" ]] && echo "Custom Iterations: ${ITERATIONS#-n }"
        echo

        # Run perf with optional iteration count
        sudo perf stat -r 5 ./build/bin/benchmark "$POLY_TYPE" "$COMP_FUNC" $ITERATIONS 2>&1 | tee "$OUTPUT_FILE"
        echo "Results saved to: $OUTPUT_FILE"
        echo "-------------------------------------------------"
    done
done

echo "All tests completed. Results are in: $OUTPUT_DIR"
