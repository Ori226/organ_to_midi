#!/bin/bash

# Configuration
BRIDGE_SCRIPT="$(pwd)/simple_midi_bridge.py"
SOUNDFONT="/usr/share/sounds/sf2/FluidR3_GM.sf2"

echo "Stopping old processes..."
pkill -f simple_midi_bridge.py
pkill fluidsynth
sleep 1

echo "Starting FluidSynth..."
nohup fluidsynth -is -a alsa -m alsa_seq "$SOUNDFONT" > fluidsynth.log 2>&1 &
sleep 2

echo "Starting MIDI Bridge..."
nohup python3 -u "$BRIDGE_SCRIPT" > midi_bridge.log 2>&1 &
sleep 3

echo "Connecting Ports..."
# Use partial matches to find client IDs
BRIDGE_PORT=$(aconnect -lo | grep "RtMidi" | awk '{print $2}' | cut -d':' -f1 | head -n 1)
SYNTH_PORT=$(aconnect -li | grep "FLUID Synth" | awk '{print $2}' | cut -d':' -f1 | head -n 1)

if [ -n "$BRIDGE_PORT" ] && [ -n "$SYNTH_PORT" ]; then
    if aconnect "$BRIDGE_PORT:0" "$SYNTH_PORT:0" 2>/dev/null; then
        echo "SUCCESS: Connected 'Arduino Bridge' ($BRIDGE_PORT) to 'FLUID Synth' ($SYNTH_PORT)"
    else
        echo "ERROR: Failed to connect ports."
    fi
else
    echo "ERROR: Could not find MIDI ports. Check logs."
    echo "BRIDGE_PORT: $BRIDGE_PORT"
    echo "SYNTH_PORT: $SYNTH_PORT"
fi

echo "Organ is READY!"
