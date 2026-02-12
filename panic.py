
import mido
import time

# Configuration
MIDI_PORT_NAME = 'Organ Mock' 
# Must match the port name you connected Yoshimi to!
# Ideally we should send to ALL ports or the specific one.
# If 'Organ Mock' is gone (because the script died), we might need to recreate it momentarily 
# just to send the off signal, OR finding the Yoshimi input port directly.
# Let's try to find available output ports first.

def panic():
    print("Sending MIDI PANIC (All Notes Off)...")
    
    # Try to open the same virtual port name mainly, Yoshimi might still be connected to it?
    # If the previous script died, 'Organ Mock' port ceased to exist. 
    # Yoshimi might be orphaned.
    # We should create the port again, hope Yoshimi reconnects (unlikely automatically), 
    # OR connect to Yoshimi's port directly if listed.
    
    outputs = mido.get_output_names()
    print("Available MIDI Outputs:", outputs)
    
    # If using virtual port logic:
    try:
        with mido.open_output(MIDI_PORT_NAME, virtual=True) as outport:
            # Send All Notes Off (CC 123) and All Sound Off (CC 120) on all 16 channels
            for ch in range(16):
                # All Sound Off
                outport.send(mido.Message('control_change', channel=ch, control=120, value=0))
                # All Notes Off
                outport.send(mido.Message('control_change', channel=ch, control=123, value=0))
            print("Panic signals sent via 'Organ Mock'. Re-connect Yoshimi if needed.")
            time.sleep(1) # unexpected wait to allow transmission
            
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    panic()
