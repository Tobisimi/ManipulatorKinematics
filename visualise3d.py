import csv
import subprocess
import sys
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

if "--view-only" not in sys.argv:
    try:
        subprocess.run(["./kinematics3d.exe"], check=True)
    except subprocess.CalledProcessError:
        print("Simulation stopped. Correct the input and run again.")
        raise SystemExit(1)

frames = {}
all_x = []
all_y = []
all_z = []

with open("path3d.csv", newline="") as file:
    reader = csv.DictReader(file)

    for row in reader:
        frame = int(row["frame"])
        x = float(row["x"])
        y = float(row["y"])
        z = float(row["z"])

        if frame not in frames:
            frames[frame] = []

        frames[frame].append((x, y, z))
        all_x.append(x)
        all_y.append(y)
        all_z.append(z)

frame_numbers = sorted(frames)


def shape_coordinates(points):
    xs = [point[0] for point in points]
    ys = [point[1] for point in points]
    zs = [point[2] for point in points]

    if len(points) >= 3:
        xs.append(xs[0])
        ys.append(ys[0])
        zs.append(zs[0])

    return xs, ys, zs


original_x, original_y, original_z = shape_coordinates(
    frames[frame_numbers[0]]
)

final_x, final_y, final_z = shape_coordinates(
    frames[frame_numbers[-1]]
)

fig = plt.figure()
ax = fig.add_subplot(projection="3d")

ax.plot(
    original_x, original_y, original_z,
    "o--", color="blue", label="Original"
)

ax.plot(
    final_x, final_y, final_z,
    "o--", color="gray", label="Final"
)

moving_shape = ax.plot(
    original_x, original_y, original_z,
    "o-", color="red", label="Moving shape"
)[0]

x_middle = (min(all_x) + max(all_x)) / 2
y_middle = (min(all_y) + max(all_y)) / 2
z_middle = (min(all_z) + max(all_z)) / 2

span = max(
    max(all_x) - min(all_x),
    max(all_y) - min(all_y),
    max(all_z) - min(all_z),
    1.0
)

half_width = span * 0.6

ax.set_xlim(x_middle - half_width, x_middle + half_width)
ax.set_ylim(y_middle - half_width, y_middle + half_width)
ax.set_zlim(z_middle - half_width, z_middle + half_width)

ax.set_box_aspect((1, 1, 1))

ax.set_xlabel("x")
ax.set_ylabel("y")
ax.set_zlabel("z")
ax.set_title("3D Shape Transformation")
ax.legend()


def update(index):
    frame = frame_numbers[index]
    xs, ys, zs = shape_coordinates(frames[frame])
    moving_shape.set_data_3d(xs, ys, zs)


animation = FuncAnimation(
    fig,
    update,
    frames=len(frame_numbers),
    interval=50,
    repeat=False
)

plt.show()