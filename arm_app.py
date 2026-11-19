import math
import subprocess
import sys
import tkinter as tk
from tkinter import ttk, messagebox
from pathlib import Path

# Locate the folder containing this Python file.
folder = Path(__file__).resolve().parent

# Create the main window.
window = tk.Tk()
window.title("Two-Link Arm Simulator")

# Each pair contains a label and its starting value.
fields = [
    ("Base x", "0"),
    ("Base y", "0"),
    ("Link 1 length", "2"),
    ("Link 2 length", "1"),
    ("Shoulder angle (degrees)", "90"),
    ("Elbow angle (degrees)", "90"),
]
entries = []

# Create one label and one input box for each field.
for row, (label, default) in enumerate(fields):
    ttk.Label(window, text=label).grid(
        row=row, column=0, padx=12, pady=6, sticky="w"
    )
    entry = ttk.Entry(window, width=20)
    entry.insert(0, default)
    entry.grid(row=row, column=1, padx=12, pady=6)
    entries.append(entry)

# No animation window has been launched yet.
viewer = None

def simulate():
    global viewer

    # Finish viewing the previous animation before starting another.
    if viewer is not None and viewer.poll() is None:
        messagebox.showinfo(
            "Animation open",
            "Close the animation window before simulating again."
        )
        return

    try:
        values = [float(entry.get()) for entry in entries]
    except ValueError:
        messagebox.showerror("Invalid input", "Enter a number in every box.")
        return

    if not all(math.isfinite(value) for value in values):
        messagebox.showerror("Invalid input", "Enter finite numbers only.")
        return

    # Match the order in which C++ reads the six numbers.
    input_text = " ".join(str(value) for value in values) + "\n"

    try:
        subprocess.run(
            [str(folder / "arm.exe")],
            input=input_text,
            text=True,
            capture_output=True,
            check=True,
            cwd=folder,
        )
        viewer = subprocess.Popen(
            [
                sys.executable,
                str(folder / "visualise_arm.py"),
                "--view-only",
            ],
            cwd=folder,
        )
    except subprocess.CalledProcessError as error:
        messagebox.showerror(
            "Simulation failed",
            error.stderr.strip() or "The C++ calculation failed."
        )
    except OSError as error:
        messagebox.showerror("Could not start the program", str(error))

ttk.Button(window, text="Simulate", command=simulate).grid(
    row=len(fields), column=0, columnspan=2, pady=15
)

window.mainloop()