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

## `button_piano.ino`
**Description**: Triggers a single MIDI Note (Middle C / Note 60) when a button is pressed.
**Purpose**: Tests Digital I/O reading, Debouncing logic, and MIDI Event triggers.

### Hardware Setup
You need a simple push button.
1.  **Pin Connection**: Connect one leg of the button to **Digital Pin 2**.
2.  **Ground Connection**: Connect the other leg of the button to any **GND** pin.
3.  **No Resistor Needed**: The code enables the internal pull-up resistor.

### How to Run
Follow the same steps as above, but use `button_piano.ino`:

```bash
# Verify wiring first!
cp examples/button_piano.ino src/main.cpp

## `three_buttons_direct.ino`
**Description**: Triggers MIDI Notes 60, 61, 62 using 3 separate buttons.
**Purpose**: Easy development setup for testing chords or sequences without shift registers.

### Hardware Setup
1.  **Button 1**: Pin 2 <-> GND
2.  **Button 2**: Pin 3 <-> GND
3.  **Button 3**: Pin 4 <-> GND
4.  **Resistors**: None (Internal Pull-up enabled).

### How to Run
```bash
cp examples/three_buttons_direct.ino src/main.cpp
~/.local/bin/pio run -t upload
```

