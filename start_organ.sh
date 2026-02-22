#!/bin/bash

# Configuration
BRIDGE_SCRIPT="$(pwd)/simple_midi_bridge.py"
SOUNDFONT="$(pwd)/theremin.sf2"

echo "Stopping old processes..."
pkill -9 -f simple_midi_bridge.py
pkill -9 fluidsynth
sleep 2

echo "Starting FluidSynth with PipeWire..."
# Try pipewire first, then pulseaudio as fallback
nohup fluidsynth -is -a pipewire -m alsa_seq "$SOUNDFONT" > fluidsynth.log 2>&1 &
sleep 2

# Check if it started, if not try pulseaudio
if ! pgrep fluidsynth > /dev/null; then
    echo "PipeWire failed, trying PulseAudio..."
    nohup fluidsynth -is -a pulseaudio -m alsa_seq "$SOUNDFONT" >> fluidsynth.log 2>&1 &
    sleep 2
fi

echo "Starting MIDI Bridge..."
nohup python3 -u "$BRIDGE_SCRIPT" > midi_bridge.log 2>&1 &
sleep 2

echo "Connecting Ports..."
for i in {1..10}; do
    BRIDGE_PORT=$(aconnect -l | grep "RtMidi" | awk '{print $2}' | cut -d':' -f1 | head -n 1)
    SYNTH_PORT=$(aconnect -l | grep "FLUID Synth" | awk '{print $2}' | cut -d':' -f1 | head -n 1)

    if [ -n "$BRIDGE_PORT" ] && [ -n "$SYNTH_PORT" ]; then
        if aconnect "$BRIDGE_PORT:0" "$SYNTH_PORT:0" 2>/dev/null; then
            echo "SUCCESS: Connected 'Arduino Bridge' ($BRIDGE_PORT) to 'FLUID Synth' ($SYNTH_PORT)"
            break
        elif aconnect "$SYNTH_PORT:0" "$BRIDGE_PORT:0" 2>/dev/null; then
            echo "SUCCESS: Connected (reverse) 'FLUID Synth' ($SYNTH_PORT) to 'Arduino Bridge' ($BRIDGE_PORT)"
            break
        fi
    fi
    echo "Attempt $i: Ports not ready yet..."
    sleep 2
done

echo "Organ is READY!"
