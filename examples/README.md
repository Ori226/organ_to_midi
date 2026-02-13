# Project Examples

This directory contains standalone example sketches to demonstrate specific features of the hardware and software stack.

## `rhythm_sim.ino`
**Description**: Simulates a pianist playing a specific rhythm (Beethoven's 5th motif).
**Purpose**: Verifies that the Arduino -> Serial -> MIDI Bridge -> FluidSynth pipeline is working correctly without needing the full organ hardware connected.

### How to Run on Remote Machine (using PlatformIO)

Since this is a standalone `.ino` file, the easiest way to run it with our existing PlatformIO setup is to likely overwrite the main source file temporarily or create a new project.

**Option 1: Quick Overwrite (Recommended for Testing)**
Run this logic on the remote machine (e.g., `old_asus`):

```bash
# 1. Back up the main project if needed
mv src/main.cpp src/main.cpp.bak

# 2. Copy the example to the source directory
cp examples/rhythm_sim.ino src/main.cpp

# 3. Upload
~/.local/bin/pio run -t upload
```

**Option 2: Create a Dedicated Test Project**
If you want to keep the main project clean:

```bash
# 1. Create a new directory
mkdir -p midi_test
cd midi_test

# 2. Initialize PlatformIO project for Arduino Uno
~/.local/bin/pio project init --board uno

# 3. Copy the example code
cp ../examples/rhythm_sim.ino src/main.cpp

# 4. Upload
~/.local/bin/pio run -t upload
```
