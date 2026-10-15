"""Plot the CSV printed by sigproc_demo.

Usage:
    ./build/dev/examples/sigproc_demo > demo.csv
    python scripts/plot_demo.py demo.csv docs/demo.png

Needs matplotlib:  pip install matplotlib
"""
import csv
import sys

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt  # noqa: E402


def main(csv_path: str, out_path: str) -> None:
    with open(csv_path, newline="") as f:
        rows = list(csv.DictReader(f))
    t = [float(r["t"]) for r in rows]

    fig, ax = plt.subplots(figsize=(10, 4.5))
    ax.plot(t, [float(r["raw"]) for r in rows], ".", color="#b0b7c3", markersize=4, label="raw (noise + spikes)")
    ax.plot(t, [float(r["moving_average"]) for r in rows], color="#e08a1e", linewidth=1.6, label="moving average (9)")
    ax.plot(t, [float(r["median"]) for r in rows], color="#1a7f37", linewidth=1.8, label="median (9)")
    ax.plot(t, [float(r["low_pass"]) for r in rows], color="#2c6fbb", linewidth=1.6, label="low-pass (2 Hz)")
    ax.set_xlabel("time (s)")
    ax.set_ylabel("signal")
    ax.set_title("Three filters on the same noisy signal. The median removes the spikes.")
    ax.grid(alpha=0.25)
    ax.legend(loc="upper right", fontsize=9)
    fig.tight_layout()
    fig.savefig(out_path, dpi=120)


if __name__ == "__main__":
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    main(sys.argv[1], sys.argv[2])
