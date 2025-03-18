import argparse
import subprocess
import json
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path
import perf_data_cleaner as pdc


def parse_arguments():
    parser = argparse.ArgumentParser(
        description="Runs perf tests for various combinations of polymorphism "
                    "types and compute functions.\n"
                    "Saves raw (.txt) and cleaned (.feather) data to a "
                    "timestamped directory under ./data/perf."
    )
    parser.add_argument(
        "-p", "--polymorphism_types",
        nargs="+",
        default=["crtp", "concepts", "runtime"],
        help="List of polymorphism types to test (default: crtp concepts runtime)",
    )
    parser.add_argument(
        "-c", "--compute_functions",
        nargs="+",
        default=["fma", "expensive"],
        help="List of compute functions to test (default: fma expensive)",
    )
    parser.add_argument(
        "-r", "--runs_per_condition",
        type=int,
        default=5,
        help="Number of runs per test condition (default: 5)",
    )
    parser.add_argument(
        "-i", "--iterations_per_run",
        type=int,
        default=1000000000,
        help="Number of iterations per run (default: 1000000000)",
    )
    return parser.parse_args()


@dataclass
class TestCondition:
    polymorphism_type: str
    compute_function: str
    num_iterations_per_run: int = 1000000000
    num_runs: int = 5


class PerfTestRunner:
    _default_data_dir = Path(__file__).parent.parent.parent / "data" / "perf"
    _ignored_perf_categories = ["topdown_events"]

    def __init__(
        self,
        test_condition: TestCondition,
        perf_categories: list[str] = None,
        extra_perf_events: list[str] = None,
        test_categories_json: Path = Path(__file__).parent
        / "test_categories.json",
        perf_events_json: Path = Path(__file__).parent / "perf_events.json",
        output_dir: Path = None,
        seq_id: int = 1,
        output_filename: str = None,
        summary_output_filename: str = None,
    ):
        self.test_condition = test_condition
        self.perf_events_json = perf_events_json
        self.perf_categories = perf_categories or [
            item
            for item in self.standard_perf_events.keys()
            if item not in self._ignored_perf_categories
        ]
        self.extra_perf_events = extra_perf_events or []
        self.test_categories_json = test_categories_json
        self.output_dir = output_dir or self.create_output_directory()
        self.seq_id = seq_id
        self.output_filename = output_filename or self.create_output_filename()
        self.summary_output_filename = (
            summary_output_filename
            or self.create_output_filename(is_summary=True)
        )

    @property
    def polymorphism_type(self) -> str:
        return self.test_condition.polymorphism_type

    @property
    def compute_function(self) -> str:
        return self.test_condition.compute_function

    @property
    def num_iterations_per_run(self) -> int:
        return self.test_condition.num_iterations_per_run

    @property
    def num_runs(self) -> int:
        return self.test_condition.num_runs

    @staticmethod
    def create_output_directory() -> Path:
        """Creates a timestamped directory for storing results and returns its path."""
        timestamp = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
        output_dir = PerfTestRunner._default_data_dir / timestamp
        output_dir.mkdir(parents=True, exist_ok=True)
        return output_dir

    def create_output_filename(self, is_summary: bool = False) -> str:
        """Creates a filename based on the polymorphism type and compute function."""
        suffix = "_summary" if is_summary else ""
        return f"{self.seq_id}_{self.polymorphism_type}_{self.compute_function}{suffix}.txt"

    @property
    def output_path(self) -> Path:
        """Returns the full output path for the results file."""
        return self.output_dir / self.output_filename

    @property
    def summary_output_path(self) -> Path:
        """Returns the full output path for the summary results file."""
        return self.output_dir / self.summary_output_filename

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

    @property
    def summary_command(self) -> list[str]:
        return [
            "sudo",
            "perf",
            "stat",
            "-r",
            str(self.num_runs),
            "./build/bin/benchmark",
            self.polymorphism_type,
            self.compute_function,
            "-n",
            str(self.num_iterations_per_run),
        ]

    @property
    def test_run_header(self) -> str:
        return (
            f"\n------------------------------------------------\n"
            f"Running perf for: Polymorphism Type = {self.polymorphism_type}\n"
            f"Compute Function = {self.compute_function}\n"
            f"Number of Runs = {self.num_runs}\n"
            f"Number of Iterations per Run = {self.num_iterations_per_run}"
        )

    def run_tests(self):
        if not self.is_valid_test:
            raise ValueError("Invalid test configuration")

        print(self.test_run_header)

        print("Running perf to collect detailed event data...")
        with self.output_path.open(mode="w") as output_file:
            process = subprocess.run(
                self.command,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
            )
            output_file.write(process.stdout)
        print(f"Results saved to: {self.output_path}")

        print("Re-running perf to collect summary data...")
        with self.summary_output_path.open(mode="w") as summary_file:
            process = subprocess.run(
                self.summary_command,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
            )
            summary_file.write(process.stdout)
        print(f"Results saved to: {self.summary_output_path}")


class MultiTestRunner:
    def __init__(
        self,
        polymorphism_types: list[str],
        compute_functions: list[str],
        num_runs_per_condition: int = 5,
        num_iterations_per_run: int = 1000000000,
    ):
        self.polymorphism_types = polymorphism_types
        self.compute_functions = compute_functions
        self.num_runs_per_condition = num_runs_per_condition
        self.num_iterations_per_run = num_iterations_per_run
        self.output_dir = PerfTestRunner.create_output_directory()

    @property
    def test_conditions(self) -> list[TestCondition]:
        return [
            TestCondition(
                polymorphism_type=pt,
                compute_function=cf,
                num_runs=self.num_runs_per_condition,
                num_iterations_per_run=self.num_iterations_per_run,
            )
            for pt in self.polymorphism_types
            for cf in self.compute_functions
        ]

    @property
    def test_runners(self) -> list[PerfTestRunner]:
        runner_list = []
        for idx in range(len(self.test_conditions)):
            runner_list.append(
                PerfTestRunner(
                    test_condition=self.test_conditions[idx],
                    output_dir=self.output_dir,
                    seq_id=idx + 1,
                )
            )
        return runner_list

    def run_tests(self):
        for test_runner in self.test_runners:
            test_runner.run_tests()


if __name__ == "__main__":
    args = parse_arguments()

    multi_test_runner = MultiTestRunner(
        polymorphism_types=args.polymorphism_types,
        compute_functions=args.compute_functions,
        num_runs_per_condition=args.runs_per_condition,
        num_iterations_per_run=args.iterations_per_run,
    )
    multi_test_runner.run_tests()

    pdc.build_detail_and_summary_dfs(data_dir=multi_test_runner.output_dir)
