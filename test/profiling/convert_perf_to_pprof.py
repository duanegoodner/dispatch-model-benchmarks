import subprocess
from pathlib import Path
import argparse


def convert_perf_data(perf_data_path: Path, executable_path: Path):
    """Convert perf.data to pprof-compatible profile.pb.gz format."""
    output_pprof_file = perf_data_path.with_suffix(".pb.gz")

    perf_script_cmd = f"perf script -i {perf_data_path}"
    pprof_cmd = f"pprof --symbolize --proto {executable_path}"

    full_command = f"{perf_script_cmd} | {pprof_cmd} > {output_pprof_file}"

    print(f"🔄 Converting: {perf_data_path.name} -> {output_pprof_file.name}")

    try:
        subprocess.run(
            full_command, shell=True, check=True, executable="/bin/bash"
        )
        print(f"✅ Successfully converted: {output_pprof_file}")
    except subprocess.CalledProcessError as e:
        print(f"❌ Error converting {perf_data_path.name}: {e}")


def process_directory(directory: Path):
    """Process all perf .data files in the given directory."""
    benchmark_executable = directory / "benchmark"

    # Ensure benchmark executable exists
    if not benchmark_executable.exists():
        print(f"⚠️ Benchmark executable not found in {directory}. Skipping.")
        return

    # Find all perf .data files
    perf_data_files = list(directory.glob("perf_*.data"))

    if not perf_data_files:
        print(f"⚠️ No perf .data files found in {directory}.")
        return

    for perf_data_file in perf_data_files:
        convert_perf_data(perf_data_file, benchmark_executable)


def main():
    parser = argparse.ArgumentParser(
        description="Convert perf .data files to pprof .pb.gz format."
    )
    parser.add_argument(
        "directory",
        type=str,
        help="Path to the directory containing perf .data files and the benchmark executable.",
    )
    args = parser.parse_args()

    target_directory = Path(args.directory)

    if not target_directory.exists() or not target_directory.is_dir():
        print(
            f"❌ Error: Directory {target_directory} does not exist or is not a directory."
        )
        return

    process_directory(target_directory)


if __name__ == "__main__":
    main()
