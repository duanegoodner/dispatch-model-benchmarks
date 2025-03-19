import argparse
import project_builder as pb
import perf_tests as pt
import perf_data_cleaner as pdc


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Run performance tests for multiple build configurations."
    )
    parser.add_argument(
        "-o",
        "--optimization_levels",
        type=str,
        nargs="+",
        default=["O0", "O1", "O2", "O3"],
        help="List of optimization levels to test (default = O0, O1, O2, O3).",
    )
    parser.add_argument(
        "-p",
        "--polymorphism_types",
        type=str,
        nargs="+",
        default=["crtp", "concepts", "runtime"],
        help="List of polymorphism types to test (default = crtp, concepts, runtime).",
    )
    parser.add_argument(
        "-c",
        "--compute_functions",
        type=str,
        nargs="+",
        default=["fma", "expensive"],
        help="List of compute functions to test (default = fma, expensive).",
    )
    parser.add_argument(
        "-r",
        "--num_runs_per_condition",
        type=int,
        default=5,
        help="Number of runs per condition (default = 5).",
    )
    parser.add_argument(
        "-i",
        "--num_iterations_per_run",
        type=int,
        default=1000000000,
        help="Number of iterations per run (default = 1000000000).",
    )
    return parser.parse_args()


class MultiBuildPerfTester:
    def __init__(
        self,
        optimization_levels,
        polymorphism_types: list[str],
        compute_functions: list[str],
        num_runs_per_condition: int,
        num_iterations_per_run: int = 1000000000,
    ):
        self.optimization_levels = optimization_levels
        self.polymorphism_types = polymorphism_types
        self.compute_functions = compute_functions
        self.num_runs_per_condition = num_runs_per_condition
        self.num_iterations_per_run = num_iterations_per_run

    def run_tests(self):
        for level in self.optimization_levels:
            print(f"Running tests for optimization level: {level}")
            builder = pb.ProjectBuilder(level)
            builder.configure()
            builder.build()

            multi_test_runner = pt.MultiTestRunner(
                polymorphism_types=self.polymorphism_types,
                compute_functions=self.compute_functions,
                num_runs_per_condition=self.num_runs_per_condition,
                dir_suffix=level,
                num_iterations_per_run=self.num_iterations_per_run,
            )

            if builder.binary_size is not None:
                print(
                    f"Binary size for optimization level{level}: {builder.binary_size} bytes "
                    f"({builder.binary_size / 1024:.2f} KB)"
                )
                size_output_path = multi_test_runner.output_dir / "binary_size.txt"
                with open(size_output_path, mode="w") as f:
                    f.write(
                        f"{builder.binary_size} bytes\n"
                        f"{builder.binary_size / 1024:.2f} KB"
                    )

            multi_test_runner.run_tests()
            pdc.build_detail_and_summary_dfs(
                data_dir=multi_test_runner.output_dir
            )


if __name__ == "__main__":

    args = parse_arguments()

    mult_build_tester = MultiBuildPerfTester(
        optimization_levels=args.optimization_levels,
        polymorphism_types=args.polymorphism_types,
        compute_functions=args.compute_functions,
        num_runs_per_condition=args.num_runs_per_condition,
        num_iterations_per_run=args.num_iterations_per_run,
    )

    mult_build_tester.run_tests()
