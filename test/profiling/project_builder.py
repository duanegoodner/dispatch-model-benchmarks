from pathlib import Path
import subprocess


class ProjectBuilder:
    """Encapsulates the logic for building a CMake project with a specified optimization level."""

    def __init__(
        self,
        optimization_level: str,
        binary_dirname: str = "bin",
        binary_filename: str = "benchmark",
    ):
        """
        Initializes the ProjectBuilder with an optimization level.
        Forces all other CMake options to OFF.
        """
        if optimization_level not in {"O0", "O1", "O2", "O3"}:
            raise ValueError(
                f"Invalid optimization level: {optimization_level}"
            )

        self.optimization_level = optimization_level
        self.binary_dirname = binary_dirname
        self.binary_filename = binary_filename

        # Adjust paths since this script runs from ./test/profiling/
        self.project_root = (
            Path(__file__).resolve().parents[2]
        )  # Move up from ./test/profiling/ to project root
        self.build_dir = self.project_root / "build"

        # Define all other CMake options (explicitly set to OFF)
        self.cmake_options = {
            "RESET_DEFAULTS": "OFF",
            "ENABLE_DEBUG": "OFF",
            "ENABLE_NO_INLINE": "OFF",
            "ENABLE_PROFILING": "OFF",
            "ENABLE_CONCEPT_ERROR_DETAIL": "OFF",
        }

    @property
    def binary_path(self) -> Path:
        return self.build_dir / self.binary_dirname / self.binary_filename

    @property
    def binary_size(self) -> int | None:
        if self.binary_path.exists():
            return self.binary_path.stat().st_size

    def configure(self):
        """Runs CMake to configure the project with the specified optimization level."""
        print(
            f"\n🔧 Configuring project with -{self.optimization_level} optimization...\n"
        )

        # Remove the existing build directory to ensure a clean build
        if self.build_dir.exists():
            subprocess.run(["rm", "-rf", str(self.build_dir)])

        # Construct the CMake command
        cmake_cmd = [
            "cmake",
            "-B",
            str(self.build_dir),
            "-S",
            str(self.project_root),
            "-DCMAKE_BUILD_TYPE=Release",
        ]

        # Enable the selected optimization level and explicitly disable others
        for level in {"O0", "O1", "O2", "O3"}:
            cmake_cmd.append(
                f"-DENABLE_{level}={'ON' if level == self.optimization_level else 'OFF'}"
            )

        # Append all other CMake options explicitly
        for option, value in self.cmake_options.items():
            cmake_cmd.append(f"-D{option}={value}")

        # Run CMake configuration
        subprocess.run(cmake_cmd, check=True)

    def build(self):
        """Builds the project."""
        print(
            f"🚀 Building project with -{self.optimization_level} optimization...\n"
        )
        build_cmd = ["cmake", "--build", str(self.build_dir)]
        subprocess.run(build_cmd, check=True)
        print(f"✅ Build completed for -{self.optimization_level}\n")


if __name__ == "__main__":
    # Example usage
    my_optimization_level = (
        "O2"  # Change this to the desired optimization level
    )
    builder = ProjectBuilder(my_optimization_level)
    builder.configure()
    builder.build()
    print("Project built successfully.")
