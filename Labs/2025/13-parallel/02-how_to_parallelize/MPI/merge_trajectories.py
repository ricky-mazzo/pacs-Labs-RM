
import pandas as pd
import glob
import re
import os

def extract_info(filename):
    match = re.search(r'trajectory_rank_(\d+)_t(\d+)\.csv', filename)
    if match:
        return int(match.group(1)), int(match.group(2))
    return -1, -1

files = glob.glob("trajectory_rank_*_t*.csv")

timestep_dict = {}
for f in files:
    rank, timestep = extract_info(f)
    if timestep not in timestep_dict:
        timestep_dict[timestep] = []
    timestep_dict[timestep].append((rank, f))

output_dir = "merged_trajectories"
os.makedirs(output_dir, exist_ok=True)

for timestep in sorted(timestep_dict.keys()):
    frames = []
    for rank, f in sorted(timestep_dict[timestep]):
        df = pd.read_csv(f, header=None, names=["x", "y"])
        df["rank"] = rank
        df["timestep"] = timestep
        frames.append(df)
    merged = pd.concat(frames, ignore_index=True)
    merged.to_csv(f"{output_dir}/step_{timestep}.csv", index=False)

print("Merged trajectory files saved in:", output_dir)
