# Agentic Installation & Setup Guide

This document provides a precise, step-by-step procedure for an AI agent to set up the environment on a target Linux machine.

## 1. Prerequisites Check
Ensure the following tools are available or install them.

```bash
# Check for optional tools (install if missing)
command -v git >/dev/null 2>&1 || sudo apt-get install -y git
command -v pipx >/dev/null 2>&1 || sudo apt-get install -y pipx
```

## 2. User Permissions
Ensure the current user has access to serial ports (for Arduino) and audio (for MIDI).

```bash
# Add user to 'dialout' (Serial) and 'audio' (Sound) groups
sudo usermod -aG dialout,audio $USER

# NOTE: Group changes require a logout/login to take effect. 
# For current shell session, use 'newgrp dialout' if needed.
```

## 3. PlatformIO Installation (Isolated)
Install PlatformIO using `pipx` to avoid system python conflicts.

```bash
pipx install platformio
pipx ensurepath
source ~/.profile  # or ~/.bashrc to load PATH changes
```

## 4. Repository Setup
Clone the project repository.

```bash
git clone https://github.com/Ori226/organ_to_midi.git ~/organ_to_midi
cd ~/organ_to_midi
```

## 5. Python MIDI Dependencies
Install Python libraries required for the MIDI bridge and mocking tools.

```bash
# Install system-level python packages (preferred for stability)
sudo apt-get install -y python3-mido python3-rtmidi python3-serial
```

## 6. Software Synthesizer Setup (Remote Machine)
Install FluidSynth and a soundfont to generate audio from MIDI.

```bash
sudo apt-get update
sudo apt-get install -y fluidsynth fluid-soundfont-gm alsa-utils
```

### Start Synthesizer Service
Run FluidSynth in server mode with ALSA drivers.

```bash
# Run in background (screen or nohup recommended for persistence)
fluidsynth -is -a alsa -m alsa_seq /usr/share/sounds/sf2/FluidR3_GM.sf2 &
```

## 7. Verification & MIDI Bridge
Run the following steps to verify the complete pipeline.

### Step 1: Check Serial Output
Verify the Arduino is sending MIDI data over USB Serial.

```bash
# Expect output like: 90 3c 7f ... (Note On)
stty -F /dev/ttyACM0 115200 raw
timeout 3s cat /dev/ttyACM0 | xxd
```

### Step 2: Run MIDI Bridge
Execute the Python bridge script to forward Serial MIDI to the OS MIDI system.

```bash
# Ensure you are in the virtual environment or have libraries installed
python3 organ_to_midi/simple_midi_bridge.py
```

### Step 3: Connect to Synthesizer
Once the bridge is running, connect its output to FluidSynth.

```bash
# List clients to find IDs
aconnect -l

# Connect Bridge (sender) to FluidSynth (receiver)
# Example: aconnect 128:0 129:0
aconnect "Arduino Bridge":0 "FLUID Synth":0
```
