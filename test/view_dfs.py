import pyarrow.feather as feather
from pathlib import Path


if __name__ == "__main__":
    data_dir = (
        Path(__file__).parent.parent / "data" / "perf" / "2025-03-17_10-32-06"
    )
    detailed_df = feather.read_feather(data_dir / "perf_detailed_runs.feather")
    summary_df = feather.read_feather(data_dir / "perf_summary_runs.feather")

    summary_df.sort_values(by="Test Name", ascending=False, inplace=True)
    detailed_df.sort_values(by="Test Name", ascending=False, inplace=True)

    basic_work = detailed_df[
        [
            "Test Name",
            "Mean Time",
            "branches",
            "instructions",
            "uops_issued.any",
        ]
    ]

    basic_work_formatted = basic_work.map(
        lambda x: f"{x:.3e}" if isinstance(x, int) else x
    )
    print(basic_work_formatted.to_markdown(index=False))

    branch_info = detailed_df[
        ["Test Name", "branches", "branch-misses"]
    ].copy()
    branch_info["miss_fraction"] = (
        branch_info["branch-misses"] / branch_info["branches"]
    )
    branch_info_formatted = branch_info.map(
        lambda x: f"{x:.3e}" if isinstance(x, int | float) else x
    )
    print(branch_info_formatted.to_markdown(index=False))

    memory_info = detailed_df[
        [
            "Test Name",
            "mem_inst_retired.all_stores",
            "mem_inst_retired.all_loads",
            "cache-misses",
        ]
    ].copy()

    memory_info["cache-miss_fraction"] = (
        memory_info["cache-misses"] / memory_info["mem_inst_retired.all_loads"]
    )

    memory_info_formatted = memory_info.copy()
    memory_info_formatted[memory_info.select_dtypes(include=['number']).columns] = (
        memory_info[memory_info.select_dtypes(include=['number']).columns]
    )
    print(memory_info_formatted.to_markdown(index=False))


