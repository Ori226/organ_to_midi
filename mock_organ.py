
import mido
import time
import random

# Configuration
MIDI_PORT_NAME = 'Organ Mock'
CHANNEL = 0 # MIDI Channel 1
VELOCITY = 127
BASE_NOTE = 60 # Middle C

def run_mock():
    print(f"Starting Organ Mock...")
    print(f"Opening Virtual MIDI Output Port: '{MIDI_PORT_NAME}'")
    
    try:
        # Open Virtual Port
        with mido.open_output(MIDI_PORT_NAME, virtual=True) as outport:
            print("Mock Running! Press Ctrl+C to stop.")
            print("Playing C Major Scale...")
            
            # C Major Scale: C, D, E, F, G, A, B, C
            scale_offsets = [0, 2, 4, 5, 7, 9, 11, 12]
            
            while True:
                for offset in scale_offsets:
                    note = BASE_NOTE + offset
                    
                    # Note On
                    print(f"-> Note ON:  {note}")
                    msg_on = mido.Message('note_on', note=note, velocity=VELOCITY, channel=CHANNEL)
                    outport.send(msg_on)
                    
                    time.sleep(0.5) # Hold note
                    
                    # Note Off
                    print(f"-> Note OFF: {note}")
                    msg_off = mido.Message('note_off', note=note, velocity=0, channel=CHANNEL)
                    outport.send(msg_off)
                    
                    time.sleep(0.1) # Short gap

    except OSError as e:
        print(f"MIDI Error: {e}")
        print("Ensure 'python-rtmidi' is installed.")
    except KeyboardInterrupt:
        print("\nStopping mock...")

if __name__ == "__main__":
    run_mock()
