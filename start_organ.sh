#!/bin/bash

# Configuration
BRIDGE_SCRIPT="$HOME/organ_to_midi/simple_midi_bridge.py"
SOUNDFONT="/usr/share/sounds/sf2/FluidR3_GM.sf2"

echo "Stopping old processes..."
pkill -f simple_midi_bridge.py
pkill fluidsynth

echo "Starting FluidSynth..."
nohup fluidsynth -is -a alsa -m alsa_seq "$SOUNDFONT" > /dev/null 2>&1 &
sleep 2

echo "Starting MIDI Bridge..."
# Use python3 directly (assuming system packages or venv is active)
# If using venv: source $HOME/organ_to_midi/venv/bin/activate
nohup python3 -u "$BRIDGE_SCRIPT" > /dev/null 2>&1 &
sleep 2

echo "Connecting Ports..."
# Loop to ensure connection
for i in {1..10}; do
    if aconnect 'RtMidiOut Client':0 'FLUID Synth':0 2>/dev/null; then
        echo "SUCCESS: Connected 'RtMidiOut Client' to 'FLUID Synth'"
        break
    else
        echo "Waiting for ports... ($i/10)"
        sleep 1
    fi
done

echo "Organ is READY!"
