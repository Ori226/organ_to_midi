# Remote Arduino Setup Walkthrough

## Environment Setup on `old_asus`
1.  **PlatformIO Installation**:
    -   Installed via `pipx` (isolated environment).
    -   Binary path: `~/.local/bin/pio`.
    -   User `ori-tal` is a member of `dialout` group (permission to access serial ports).

2.  **Repository**:
    -   Cloned to `~/organ_to_midi`.
    -   Location: `/home/ori-tal/organ_to_midi`.

## Blink Test Verification
A test project was created at `~/organ_to_midi/blink_test` to verify the toolchain and hardware connection.

### Hardware Detected
-   **Device**: Arduino Uno R3
-   **Port**: `/dev/ttyACM0`

### Test Execution
The following command was used to upload a blink firmware:
```bash
cd ~/organ_to_midi/blink_test && ~/.local/bin/pio run -t upload
```

**Result**: Success. The board should now be blinking its built-in LED (double blink pattern: ON-OFF-ON-LONG_OFF).

## Running the Main Project
To run the main organ software in the future (targeting the Mega 2560):
1.  Navigate to the repo: `cd ~/organ_to_midi`
2.  Run upload: `~/.local/bin/pio run -t upload` (Note: Ensure the board is ATmega2560, or update `platformio.ini` for the connected board).

## MIDI Sound Setup
The system is now capable of sending MIDI messages. To generate sound:

1.  **Install Synthesizer** (Requires sudo):
    ```bash
    sudo apt-get update
    sudo apt-get install -y fluidsynth fluid-soundfont-gm alsa-utils python3-mido python3-rtmidi python3-serial
    ```

2.  **Start Synthesizer**:
    ```bash
    fluidsynth -is -a alsa -m alsa_seq /usr/share/sounds/sf2/FluidR3_GM.sf2 &
    ```

3.  **Run MIDI Bridge**:
    ```bash
    # Run the bridge script (uses local venv if system libs missing)
    source ~/organ_to_midi/venv/bin/activate
    python3 organ_to_midi/simple_midi_bridge.py
    ```

4.  **Connect Ports**:
    ```bash
    aconnect "Arduino Bridge":0 "FLUID Synth":0
    ```

**Status**: Success! The MIDI bridge is connected to FluidSynth, and sound should be audible from the `old_asus` speakers (playing a note every 2 seconds).

## Stopping the Sound
To stop the audio generation, run the following command on `old_asus` (or via SSH):

```bash
pkill fluidsynth
pkill -f simple_midi_bridge.py
```

## Quick Start (Robust)
Copy-paste this block. It cleans up old processes and waits for ports to be ready.

```bash
# Cleanup old processes
pkill fluidsynth
pkill -f simple_midi_bridge.py

# Start FluidSynth (Background)
nohup fluidsynth -g 2.0 -is -a alsa -m alsa_seq /usr/share/sounds/sf2/FluidR3_GM.sf2 > /dev/null 2>&1 &

# Start MIDI Bridge (Background)
source ~/organ_to_midi/venv/bin/activate
nohup python3 -u ~/organ_to_midi/simple_midi_bridge.py > /dev/null 2>&1 &

# Wait loop for ports to appear
echo "Waiting for MIDI ports..."
for i in {1..10}; do
  if aconnect -l | grep -q "RtMidiOut Client" && aconnect -l | grep -q "FLUID Synth"; then
    echo "Ports found!"
    break
  fi
  sleep 1
done

# Connect
aconnect "RtMidiOut Client":0 "FLUID Synth":0 && echo "Connected! Sound should be active." || echo "Failed to connect."
```


## Troubleshooting & Robustness
See [Deployment Lessons](file:///home/ori/.gemini/antigravity/brain/8c48ca99-ab0f-49ba-8c43-dc35ad5a86f4/deployment_lessons.md) for details on:
1.  USB Upload Timeouts.
2.  Serial Port changes (ACM0 -> ACM1).
3.  "Silent" Organ (Floating Inputs).
4.  Persistent ALSA connections.

