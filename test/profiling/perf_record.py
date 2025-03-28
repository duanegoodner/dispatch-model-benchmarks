import subprocess
import argparse
from pathlib import Path

# Define constants
EXECUTABLE_NAME = "benchmark"  # Binary name

# Define test conditions
POLYMORPHISM_TYPES = ["concepts", "crtp", "runtime"]
COMPUTE_FUNCTIONS = ["fma", "expensive"]


def get_build_directories(base_dir: Path):
    """Retrieve all directories containing compiled benchmark executables."""
    return [d for d in base_dir.iterdir() if d.is_dir()]


def run_perf_record(binary_path: Path, poly: str, compute: str, output_file: Path):
    """
    Runs perf record for a specific binary with given polymorphism and compute function.

    Args:
        binary_path (Path): Path to the benchmark executable.
        poly (str): Polymorphism type (runtime, crtp, concepts).
        compute (str): Compute function (fma, expensive).
        output_file (Path): Path to save the perf data.
    """
    perf_command = [
        "sudo",
        "perf",
        "record",
        "-g",
        "-o",
        str(output_file),
        str(binary_path),
        poly,
        compute,
    ]

    print(f"🏃 Running: {' '.join(perf_command)}")

    try:
        subprocess.run(perf_command, check=True)
    except subprocess.CalledProcessError as e:
        print(f"❌ Error running perf record: {e}")


def profile_all_executables(base_dir: Path):
    """
    Iterates over all compiled binaries in base_dir and runs perf record for each polymorphism-compute combination.

    Args:
        base_dir (Path): Path to the directory containing all executable subdirectories.
    """
    if not base_dir.exists() or not base_dir.is_dir():
        print(f"❌ Error: Provided directory does not exist or is not a directory: {base_dir}")
        return

    build_dirs = get_build_directories(base_dir)

    for build_dir in build_dirs:
        binary_path = build_dir / EXECUTABLE_NAME

        # Ensure binary exists
        if not binary_path.exists():
            print(f"⚠️ Skipping: {binary_path} (not found)")
            continue

        for poly in POLYMORPHISM_TYPES:
            for compute in COMPUTE_FUNCTIONS:
                perf_data_path = build_dir / f"perf_{poly}_{compute}.data"
                run_perf_record(binary_path, poly, compute, perf_data_path)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run perf record for all compiled benchmark executables.")
    parser.add_argument("base_dir", type=str, help="Directory containing all subdirectories with benchmark executables.")
    args = parser.parse_args()

    base_path = Path(args.base_dir)
    profile_all_executables(base_path)

    print("\n✅ All profiling runs completed successfully!")
