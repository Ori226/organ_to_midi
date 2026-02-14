# Remote Deployment & Robustness Lessons

This document captures the issues encountered during the deployment to `old_asus` and their solutions. Use this to build a more robust startup script in the future.

## 1. USB/Serial Port Issues
### Symptoms
- **Upload Timeout:** `avrdude: stk500_recv(): programmer is not responding`.
- **Port Migration:** The Arduino moves from `/dev/ttyACM0` to `/dev/ttyACM1` after a crash or rapid replug.
- **Busy Resource:** `Device or resource busy` when trying to open the port.

### Solutions applied
- **Auto-Scanning Bridge:** The `simple_midi_bridge.py` was updated to scan `["/dev/ttyACM0", "/dev/ttyACM1", "/dev/ttyUSB0"]` automatically.
- **Hardware Reset:** Remote upload failures were only resolved by physically unplugging and replugging the USB cable. *Future Robustness:* Consider a smart USB hub that can toggle power via software (using `uhubctl`).
- **Process Cleanup:** Old processes (`minicom`, stuck scripts) lock the port. Use `fuser -k /dev/ttyACM0` or `lsof /dev/ttyACM0` to find an kill them.

## 2. 74HC165 "Silence" vs "Floating"
### Logic
- **Silence (No Sound):** Often caused by the Shift Register reading `00000000` (All Inputs LOW). Since the code only processes *Changes*, a constant state sends no MIDI.
- **Hardware Cause:** A disconnected **5V rail/wire**. Without 5V, the pull-up resistors fail, inputs float low (or stick low), and the code sees "All Buttons Pressed" forever.
- **Diagnostic:** Enable `DEBUG_MODE` to see the raw binary. If it prints `00000000` repeatedly, check the voltage rail.

## 3. ALSA Connection Persistence
### The Problem
The connection between the Bridge and FluidSynth is **not persistent**.
```bash
aconnect "RtMidiOut Client":0 "FLUID Synth":0
```
**Every time the python bridge script restarts**, the "RtMidiOut Client" is destroyed and a new one is created. The ALSA link breaks immediately.

### Robustness Fix
A startup script (`start_organ.sh`) should loop and ensure connection:
```bash
#!/bin/bash
# 1. Start Synth
fluidsynth -is -a alsa -m alsa_seq /path/to/sf2 &

# 2. Start Bridge
python3 simple_midi_bridge.py &

# 3. Wait and Connect
sleep 5
aconnect 'RtMidiOut Client':0 'FLUID Synth':0
```

## 4. Software Dependencies
- **RtMidi Client Name:** The mido library creates a client named "RtMidiOut Client" by default on Linux, *not* the name you give the port ("Arduino Bridge"). Always connect to `RtMidiOut Client`.
