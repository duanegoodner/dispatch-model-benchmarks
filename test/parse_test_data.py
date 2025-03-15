import pandas as pd
import re
import ace_tools as tools


def extract_benchmark_name(lines):
    """
    Extracts the benchmark name from perf output.
    """
    for line in lines:
        match = re.search(r"Performance counter stats for '(.*?)'", line)
        if match:
            return match.group(1)
    return "Unknown Benchmark"


def parse_perf_line(line):
    """
    Parses a single line from perf output.
    Returns extracted values if the line contains performance data, otherwise None.
    """
    perf_pattern = re.compile(
        r"([\d,]+)\s+([\w\-_./]+)\s+#\s+([\d\.\s\w/%]+)\s+\(\s*\+-\s*([\d\.%]+)\s*\)"
    )
    match = perf_pattern.search(line)

    if match:
        value = int(match.group(1).replace(",", ""))  # Convert to int
        metric = match.group(2)
        rate = match.group(3).strip()
        std_dev = match.group(4)
        return metric, value, rate, std_dev
    return None


def parse_perf_output(file_path):
    """
    Parses the perf output file and extracts relevant performance statistics.
    Returns a Pandas DataFrame.
    """
    data = []

    # Read file content
    with open(file_path, "r") as file:
        lines = file.readlines()

    # Extract benchmark name
    benchmark_name = extract_benchmark_name(lines)

    # Process each line
    for line in lines:
        parsed_data = parse_perf_line(line)
        if parsed_data:
            metric, value, rate, std_dev = parsed_data
            data.append([benchmark_name, metric, value, rate, std_dev])

    # Convert to DataFrame
    df = pd.DataFrame(
        data, columns=["Benchmark", "Metric", "Value", "Rate", "Std_Dev"]
    )
    return df


if __name__ == "__main__":
    # Example: Replace with actual file path
    file_path = "runtime_fma_perf_summary.txt"
    df_perf = parse_perf_output(file_path)

    # Display DataFrame
    tools.display_dataframe_to_user(name="Parsed Perf Data", dataframe=df_perf)
