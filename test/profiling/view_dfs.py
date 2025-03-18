from functools import cached_property

import pandas as pd
import pyarrow.feather as feather
from pathlib import Path


class PerfDataAnalyzer:
    def __init__(self, data_dir: Path):
        self.data_dir = data_dir

    @staticmethod
    def sort_df(df: pd.DataFrame) -> pd.DataFrame:
        return df.sort_values(by="Test Name", ascending=False)

    @cached_property
    def detailed_df(self) -> pd.DataFrame:
        df = feather.read_feather(self.data_dir / "perf_detailed_runs.feather")
        df = self.sort_df(df=df)
        return df

    @cached_property
    def summary_df(self) -> pd.DataFrame:
        df = feather.read_feather(self.data_dir / "perf_summary_runs.feather")
        df = self.sort_df(df=df)
        return df

    @cached_property
    def basic_work_df(self) -> pd.DataFrame:
        detailed_df_copy = self.detailed_df.copy()
        df = detailed_df_copy[
            [
                "Test Name",
                "Mean Time",
                "branches",
                "instructions",
                "uops_issued.any",
            ]
        ]
        return df

    def display_basic_work_df(self):
        basic_work_copy = self.basic_work_df.copy()
        basic_work_formatted = basic_work_copy.map(
            lambda x: f"{x:.3e}" if isinstance(x, int) else x
        )
        print(basic_work_formatted.to_markdown(index=False))

    @cached_property
    def branch_info_df(self) -> pd.DataFrame:
        df = self.detailed_df[
            ["Test Name", "branches", "branch-misses"]
        ].copy()
        df["miss_fraction"] = df["branch-misses"] / df["branches"]
        return df

    def display_branch_info_df(self):
        branch_info_copy = self.branch_info_df.copy()
        branch_info_formatted = branch_info_copy.map(
            lambda x: f"{x:.3e}" if isinstance(x, int | float) else x
        )
        print(branch_info_formatted.to_markdown(index=False))

    @cached_property
    def memory_info_df(self) -> pd.DataFrame:
        df = self.detailed_df[
            [
                "Test Name",
                "mem_inst_retired.all_stores",
                "mem_inst_retired.all_loads",
                "cache-misses",
            ]
        ].copy()

        df["cache-miss_fraction"] = (
            df["cache-misses"] / df["mem_inst_retired.all_loads"]
        )
        return df

    def display_memory_info_df(self):
        memory_info_copy = self.memory_info_df.copy()
        memory_info_formatted = memory_info_copy.map(
            lambda x: f"{x:.3e}" if isinstance(x, int | float) else x
        )

        print(memory_info_formatted.to_markdown(index=False))


if __name__ == "__main__":
    my_data_dir = (
        Path(__file__).parent.parent.parent
        / "data"
        / "perf"
        / "2025-03-18_12-35-11"
    )

    data_analyzer = PerfDataAnalyzer(data_dir=my_data_dir)
    data_analyzer.display_basic_work_df()
    data_analyzer.display_branch_info_df()
    data_analyzer.display_memory_info_df()
