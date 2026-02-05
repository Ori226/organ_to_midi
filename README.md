# Vintage Organ MIDI Retrofit

## Project Overview
This project aims to retrofit a vintage analog organ (2 Manuals + Pedals) into a MIDI controller using an Arduino-compatible microcontroller.

## Hardware Architecture
*   **Controller**: Arduino Mega 2560 or Teensy 4.1 (Simulation: Arduino Uno)
*   **Method**: Keyboard Matrix Scanning
*   **Outputs**: USB MIDI

## MIDI Channels
*   **Upper Manual**: Channel 1
*   **Lower Manual**: Channel 2
*   **(Bass) Pedals**: Channel 3

## Project Structure
*   `src/Organ_Main.ino`: Main Arduino sketch.
*   `src/MatrixScanner.h/cpp`: Matrix scanning logic.
*   `src/config.h`: Pin definitions and configuration.
*   `diagram.json`: Wokwi simulation diagram.

## Development Environment
*   Target: Arduino IDE / PlatformIO
*   Libraries: `MIDI` (`USB-MIDI` on Teensy)

## Building for Wokwi Simulation

This project configured to use a local installation of `arduino-cli` (in `./bin`) to build the firmware for the Wokwi simulator.

To rebuild the project (e.g., after modifying `src/Organ_Main.ino`), run the following command in the terminal:

```bash
# Copy latest source to temp build folder and compile
cp src/Organ_Main.ino build_temp/Organ_Main/ && ./bin/arduino-cli compile --fqbn arduino:avr:uno build_temp/Organ_Main --output-dir build
```

This command:
1.  Updates the temporary build sketch folder with your latest code.
2.  Compiles it for the **Arduino Uno**.
3.  Outputs the binary to `build/Organ_Main.ino.hex`.

## Wokwi & Antigravity Configuration

To enable simulation within the Antigravity (Gravity) IDE, the following changes and tools were added:

*   **Local Build Tools**: Since the environment lacks system-wide Arduino tools, a local copy of `arduino-cli` was installed in the `./bin` directory.
*   **Wokwi Configuration**:
    *   `diagram.json`: Define the hardware (Arduino Uno, LEDs, etc.).
    *   `wokwi.toml`: Points Wokwi to the compiled firmware path (`build/Organ_Main.ino.hex`).
    *   `libraries.txt`: Lists libraries for Wokwi to download (e.g., `MIDI` for the organ project).
*   **Build Process**: A custom build command is used to move files to a temporary folder (`build_temp`) to satisfy Arduino CLI's folder naming requirements before compiling.

## Current Exercise: Simple LED Blink with Button

A basic circuit was implemented to verify the Wokwi environment and simulating inputs:

1.  **Circuit Diagram**:
    *   **Push Button** (`btn1`): Connected in series between Arduino Pin 13 and the LED.
    *   **Connections**: Pin 13 -> Button Pin 1 -> Button Pin 2 -> LED Anode -> LED Cathode -> Resistor -> GND.
2.  **Code Behavior**: 
    *   The sketch (`src/Organ_Main.ino`) blinks Pin 13 (ON for 1s, OFF for 4s).
3.  **Result**: 
    *   The LED will only physically blink when the button is held down, gating the signal from Pin 13.
