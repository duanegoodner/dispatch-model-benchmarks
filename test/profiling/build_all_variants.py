import subprocess
import shutil
import argparse
from pathlib import Path

# Define project paths
BUILD_DIR = Path("./build")
DATA_DIR = Path("./data")
EXECUTABLE_NAME = "benchmark"

# Define build configurations
OPTIMIZATION_LEVELS = ["O0", "O1", "O2", "O3"]
DEBUG_OPTIONS = [("OFF", ""), ("ON", "_g")]  # (CMake flag, suffix for directory name)

def run_command(command, cwd=None):
    """Executes a shell command."""
    print(f"🏃 Running: {' '.join(command)}")
    subprocess.run(command, check=True, cwd=cwd)

def clean_build_directory():
    """Deletes the ./build directory to force a fresh rebuild."""
    if BUILD_DIR.exists():
        print(f"🧹 Cleaning build directory: {BUILD_DIR}")
        shutil.rmtree(BUILD_DIR)

def configure_and_build(optimization: str, debug: str):
    """
    Configures and builds the project with the given optimization level and debug setting.

    Args:
        optimization (str): Optimization level (O0, O1, O2, O3)
        debug (str): "ON" or "OFF" for ENABLE_DEBUG flag
    """
    cmake_command = [
        "cmake", "-B", str(BUILD_DIR),
        f"-DENABLE_O{optimization[-1]}=ON",
        f"-DENABLE_DEBUG={debug}"
    ]
    build_command = ["cmake", "--build", str(BUILD_DIR)]

    # Run cmake configure and build
    run_command(cmake_command)
    run_command(build_command)

def copy_binary(destination: Path):
    """Copies the compiled binary from ./build/bin/ to the specified destination."""
    binary_source = BUILD_DIR / "bin" / EXECUTABLE_NAME
    if binary_source.exists():
        print(f"📦 Copying {binary_source} -> {destination}")
        shutil.copy(binary_source, destination)
    else:
        print(f"⚠️ Warning: Binary not found at {binary_source}")

def build_all_variants(output_dir: Path):
    """Builds all optimization/debug combinations and saves them under output_dir."""
    output_dir.mkdir(parents=True, exist_ok=True)

    for optimization in OPTIMIZATION_LEVELS:
        for debug_flag, debug_suffix in DEBUG_OPTIONS:
            variant_name = f"{optimization.lower()}_marchnative{debug_suffix}"
            variant_dir = output_dir / variant_name
            variant_dir.mkdir(parents=True, exist_ok=True)

            print(f"\n🔨 Building variant: {variant_name}")

            # Clean build directory before each build
            clean_build_directory()

            # Build with current settings
            configure_and_build(optimization, debug_flag)

            # Copy the binary to the appropriate directory
            copy_binary(variant_dir / EXECUTABLE_NAME)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Build all benchmark variants and store them in a specified directory.")
    parser.add_argument("output_dir", type=str, help="Name of the directory under ./data/ to store built binaries.")
    args = parser.parse_args()

    output_path = DATA_DIR / args.output_dir
    build_all_variants(output_path)

    print("\n✅ All builds completed successfully!")
