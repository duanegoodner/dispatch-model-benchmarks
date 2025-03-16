import re
from pathlib import Path

import pandas as pd
import pyarrow.feather as feather


def parse_benchmark_results(output):
    benchmark_summary = {}

    # Extract test name and iteration count from first part
    benchmark_pattern = re.compile(
        r"Running: (?P<test_name>.*?)\n"
        r"Iteration Count: (?P<iteration_count>\d+)"
    )
    matches = benchmark_pattern.findall(output)

    if matches:
        test_name, iteration_count = matches[0]
        benchmark_summary["Test Name"] = test_name
        benchmark_summary["Mean Iteration Count"] = int(iteration_count)

    # Extract mean time and error percentage from the last part
    time_pattern = re.search(
        r"(\d+\.\d+) \+\- (\d+\.\d+) seconds time elapsed  \( \+\-\s*(\d+\.\d+)% \)",
        output,
    )
    if time_pattern:
        benchmark_summary["Mean Time"] = float(time_pattern.group(1))
        benchmark_summary["Mean Time (% error)"] = float(time_pattern.group(3))

    return benchmark_summary


def clean_metric_name(metric: str) -> str:
    return metric.replace("cpu_core/", "").rstrip("/")


def parse_performance_counters(output):
    counts_results = {}
    errors_results = {}

    # Extract the performance counter section only
    perf_section_match = re.search(
        r"Performance counter stats for .*?:\n(.*?)(?:\n\n|$)",
        output,
        re.DOTALL,
    )
    if not perf_section_match:
        return {}, {}
    perf_section = perf_section_match.group(1)

    perf_pattern = re.compile(
        r"(?P<counts>[\d,]+)\s+(?P<metric>[\w\./_\-]+)\s+(?:\( \+\-\s*(?P<error>-?\d+\.\d+)% \))?"
    )

    for match in perf_pattern.finditer(perf_section):
        counts = match.group("counts").replace(",", "")
        error = match.group("error")
        metric_name = clean_metric_name(match.group("metric"))
        counts_results[metric_name] = int(counts) if counts.isdigit() else None
        errors_results[f"{metric_name} (% error)"] = (
            float(error) if error is not None else None
        )

    return counts_results, errors_results


def perf_output_to_series(file_path: Path):
    with file_path.open(mode="r") as file:
        output = file.read()

    benchmark_summary = parse_benchmark_results(output)
    counts_results, errors_results = parse_performance_counters(output)

    combined_results = {
        **benchmark_summary,
        **counts_results,
        **errors_results,
    }
    return pd.Series(combined_results)


def stack_perf_outputs(file_paths: list[Path]) -> pd.DataFrame:
    series_list = []
    for file_path in file_paths:
        perf_series = perf_output_to_series(file_path)
        perf_series["File Path"] = str(
            file_path
        )  # Add file path to track source
        series_list.append(perf_series)

    df = pd.DataFrame(series_list)

    # Reorder columns: counts first, then errors
    count_cols = [
        col
        for col in df.columns
        if "(% error)" not in col and col != "File Path"
    ]
    error_cols = [col for col in df.columns if "(% error)" in col]
    other_cols = [
        col for col in df.columns if col not in count_cols + error_cols
    ]

    ordered_cols = other_cols + count_cols + error_cols
    df = df[ordered_cols]

    return df


def save_dataframe_to_feather(df: pd.DataFrame, output_path: Path):
    feather.write_feather(df, str(output_path))


def load_dataframe_from_feather(input_path: Path) -> pd.DataFrame:
    return feather.read_feather(input_path)


if __name__ == "__main__":
    my_data_dir = (
        Path(__file__).parent.parent / "data" / "perf" / "2025-03-16_09-37-46"
    )
    my_perf_output_files = list(my_data_dir.iterdir())
    my_feather_output_path = my_data_dir / "perf_summary.feather"

    my_dataframe = stack_perf_outputs(my_perf_output_files)
    save_dataframe_to_feather(my_dataframe, my_feather_output_path)

    # Load the dataframe back and inspect
    reloaded_dataframe = load_dataframe_from_feather(my_feather_output_path)
    print(reloaded_dataframe.head())
