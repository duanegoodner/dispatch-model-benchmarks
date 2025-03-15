import os
import subprocess
import argparse
import json
from datetime import datetime
from pathlib import Path

# Define test parameters
POLYMORPHISM_TYPES = ["runtime", "concepts", "crtp"]
COMPUTE_FUNCTIONS = ["fma", "expensive"]


class PerfTestRunner:
    _default_data_dir = Path(__file__).parent.parent / "data" / "perf"
    _ignored_perf_categories = ["topdown_events"]

    def __init__(
        self,
        polymorphism_type: str,
        compute_function: str,
        num_iterations_per_run: int = 1000000000,
        num_runs: int = 5,
        perf_categories: list[str] = None,
        extra_perf_events: list[str] = None,
        test_categories_json: Path = Path(__file__).parent
        / "test_categories.json",
        perf_events_json: Path = Path(__file__).parent / "perf_events.json",
        output_dir: Path = None,
        output_filename: str = None,
    ):
        self.num_iterations_per_run = num_iterations_per_run
        self.num_runs = num_runs
        self.polymorphism_type = polymorphism_type
        self.compute_function = compute_function
        self.perf_events_json = perf_events_json
        self.perf_categories = perf_categories or [
            item
            for item in self.standard_perf_events.keys()
            if item not in self._ignored_perf_categories
        ]
        self.extra_perf_events = extra_perf_events or []
        self.test_categories_json = test_categories_json
        self.output_dir = output_dir or self.create_output_directory()
        self.output_filename = output_filename or self.create_output_filename()

    @staticmethod
    def timestamp() -> str:
        """Returns a timestamp string."""
        return datetime.now().strftime("%Y-%m-%d_%H-%M-%S")

    def create_output_directory(self) -> Path:
        """Creates a timestamped directory for storing results and returns its path."""
        timestamp = self.timestamp()
        output_dir = PerfTestRunner._default_data_dir / timestamp
        output_dir.mkdir(parents=True, exist_ok=True)
        return output_dir

    def create_output_filename(self) -> str:
        """Creates a filename based on the polymorphism type and compute function."""
        return f"{self.polymorphism_type}_{self.compute_function}_perf_summary.txt"

    @property
    def output_path(self) -> Path:
        """Returns the full output path for the results file."""
        return self.output_dir / self.output_filename

    @property
    def standard_perf_events(self) -> dict:
        # Load perf events from JSON file
        with self.perf_events_json.open() as f:
            return json.load(f)

    @property
    def perf_events_to_run(self) -> list[str]:
        events_to_run = []
        for category in self.perf_categories:
            events_to_run.extend(self.standard_perf_events.get(category, []))
        events_to_run.extend(self.extra_perf_events)
        return events_to_run

    @property
    def test_categories(self) -> dict:
        # Load test categories from JSON file
        with self.test_categories_json.open() as f:
            return json.load(f)

    @property
    def is_valid_polymorphism_type(self) -> bool:
        return self.polymorphism_type in self.test_categories.get(
            "polymorphism_types", []
        )

    @property
    def is_valid_compute_function(self) -> bool:
        return self.compute_function in self.test_categories.get(
            "compute_functions", []
        )

    @property
    def is_valid_test(self) -> bool:
        return (
            self.is_valid_polymorphism_type or self.is_valid_compute_function
        )

    @property
    def command(self) -> list[str]:
        cmd = [
            "sudo",
            "perf",
            "stat",
            "-r",
            str(self.num_runs),
            "-e",
            ",".join(self.perf_events_to_run),
            "./build/bin/benchmark",
            self.polymorphism_type,
            self.compute_function,
            "-n",
            str(self.num_iterations_per_run),
        ]
        return cmd

    def run_tests(self):
        if not self.is_valid_test:
            raise ValueError("Invalid test configuration")

        with self.output_path.open(mode="w") as output_file:
            process = subprocess.run(
                self.command,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
            )
            output_file.write(process.stdout)
        print(f"Results saved to: {self.output_path}")


def parse_arguments():
    """Parses command-line arguments for optional iteration count."""
    parser = argparse.ArgumentParser(
        description="Run perf tests for polymorphism benchmarking."
    )
    parser.add_argument(
        "-n",
        type=int,
        help="Custom iteration count (positive integer)",
        default=None,
    )
    return parser.parse_args()


def create_output_directory():
    """Creates a timestamped directory for storing results and returns its path."""
    timestamp = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
    output_dir = f"./data/perf/{timestamp}/"
    os.makedirs(output_dir, exist_ok=True)
    return output_dir


def run_perf_test(poly_type, comp_func, iterations, output_dir):
    """Runs the perf test for a given polymorphism type and compute function."""
    output_file = os.path.join(
        output_dir, f"{poly_type}_{comp_func}_perf_summary.txt"
    )

    print("\n-------------------------------------------------")
    print(
        f"Running perf for: Polymorphism Type = {poly_type}, Compute Function = {comp_func}"
    )
    if iterations:
        print(f"Custom Iterations: {iterations}")

    # Construct perf command
    cmd = [
        "sudo",
        "perf",
        "stat",
        "-r",
        "5",
        "./build/bin/benchmark",
        poly_type,
        comp_func,
    ]

    # Add iteration count if specified
    if iterations:
        cmd.extend(["-n", str(iterations)])

    # Run the command and capture output
    with open(output_file, "w") as f:
        process = subprocess.run(
            cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True
        )
        f.write(process.stdout)
        f.write("\n-------------------------------------------------\n")

    print(f"Results saved to: {output_file}")
    print("-------------------------------------------------")


def main():
    """Main function to execute all perf tests."""
    args = parse_arguments()
    output_dir = create_output_directory()
    print(f"Results will be saved in directory: {output_dir}")

    # Run tests for all combinations of polymorphism types and compute functions
    for poly_type in POLYMORPHISM_TYPES:
        for comp_func in COMPUTE_FUNCTIONS:
            run_perf_test(poly_type, comp_func, args.n, output_dir)

    print(f"\nAll tests completed. Results are in: {output_dir}")


if __name__ == "__main__":
    # main()
    test_runner = PerfTestRunner(
        num_runs=5, polymorphism_type="crtp", compute_function="fma"
    )

    test_runner.run_tests()
