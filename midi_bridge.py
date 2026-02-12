
import socket
import mido
import sys
import time

# Configuration
WOKWI_HOST = 'localhost'
WOKWI_PORT = 4000
MIDI_PORT_NAME = 'Wokwi Bridge'

def run_bridge():
    print(f"Connecting to Wokwi Simulation at {WOKWI_HOST}:{WOKWI_PORT}...")
    
    try:
        # Create TCP Socket
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((WOKWI_HOST, WOKWI_PORT))
        print("Connected to Wokwi!")
    except ConnectionRefusedError:
        print("Error: Could not connect to Wokwi.")
        print("Make sure the Wokwi simulation is RUNNING (F1 -> Start Simulation).")
        sys.exit(1)

    print(f"Opening Virtual MIDI Output Port: '{MIDI_PORT_NAME}'...")
    
    try:
        # Open Virtual Port (Linux/Mac)
        # On Windows, this requires loopMIDI or similar, but mido handles the API calls.
        # 'virtual=True' works on Linux/Mac/rtmidi.
        with mido.open_output(MIDI_PORT_NAME, virtual=True) as outport:
            print("Bridge Running! Press Ctrl+C to stop.")
            print(f"-> Forwarding data to MIDI Port: {MIDI_PORT_NAME}")
            
            while True:
                # Read 1 byte from TCP
                data = sock.recv(1024)
                if not data:
                    print("Wokwi disconnected.")
                    break
                
                # Parse and forward MIDI
                # Since Wokwi sends raw serial bytes which ARE MIDI bytes in our code,
                # we can use a MIDI parser to ensure we send complete messages,
                # OR just forward the raw bytes to the virtual port if the OS supports raw streams.
                # However, mido expects Messages.
                # Let's use a mido Parser to convert the stream back into messages.
                
                parser = mido.Parser()
                parser.feed(data)
                
                for msg in parser:
                    print(f"Sending: {msg}")
                    outport.send(msg)
                    
    except OSError as e:
        print(f"MIDI Error: {e}")
        print("Ensure 'python-rtmidi' is installed.")
    except KeyboardInterrupt:
        print("\nStopping bridge...")
    finally:
        sock.close()

if __name__ == "__main__":
    run_bridge()
