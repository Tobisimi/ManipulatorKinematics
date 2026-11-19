import sys
import csv
import subprocess
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

if "--view-only" not in sys.argv:
    try:
        subprocess.run(["./arm.exe"], check=True)
    except subprocess.CalledProcessError:
        print("Simulation stopped. Correct the input and run again.")
        raise SystemExit(1)

frames = {}
all_x = []
all_y = []

with open("arm_path.csv", newline="") as file:
    reader = csv.DictReader(file)

    for row in reader:
        frame = int(row["frame"])
        x = float(row["x"])
        y = float(row["y"])

        if frame not in frames:
            frames[frame] = []

        frames[frame].append((x, y))
        all_x.append(x)
        all_y.append(y)

frame_numbers = sorted(frames)


def shape_coordinates(points):
    xs = [point[0] for point in points]
    ys = [point[1] for point in points]

    return xs, ys


original_x, original_y = shape_coordinates(frames[frame_numbers[0]])
final_x, final_y = shape_coordinates(frames[frame_numbers[-1]])

plt.plot(
    original_x, original_y, "o--",
    color="blue", label="Original"
)

plt.plot(
    final_x, final_y, "o--",
    color="gray", label="Final"
)

moving_shape = plt.plot(
    original_x, original_y, "o-",
    color="red", label="Moving shape", zorder=5
)[0]

plt.axhline(0, color="black", linewidth=0.8)
plt.axvline(0, color="black", linewidth=0.8)
plt.xlabel("x")
plt.ylabel("y")
plt.title("Two-Link Arm")
plt.grid(True)

plt.xlim(min(all_x) - 1, max(all_x) + 1)
plt.ylim(min(all_y) - 1, max(all_y) + 1)
plt.gca().set_aspect("equal", adjustable="box")
plt.legend()


def update(index):
    frame = frame_numbers[index]
    xs, ys = shape_coordinates(frames[frame])
    moving_shape.set_data(xs, ys)


animation = FuncAnimation(
    plt.gcf(),
    update,
    frames=len(frame_numbers),
    interval=30,
    repeat=False
)

plt.show()