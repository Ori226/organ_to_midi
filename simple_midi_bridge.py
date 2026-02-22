import time
import serial
import mido

# Configuration
BAUD_RATE = 115200
CANDIDATE_PORTS = ["/dev/ttyACM0", "/dev/ttyACM1", "/dev/ttyUSB0", "/dev/ttyUSB1"]

def run_bridge():
    ser = None
    for port in CANDIDATE_PORTS:
        print(f"Trying Serial Port {port}...")
        try:
            ser = serial.Serial(port, BAUD_RATE)
            print(f"Success! Connected to {port}")
            break
        except serial.SerialException:
            pass
    
    if ser is None:
        print("Error: No suitable serial port found.")
        return

    print("Opening Virtual MIDI Output Port...")
    # Open virtual port (visible to ALSA)
    try:
        with mido.open_output("Arduino Bridge", virtual=True) as outport:
            print("Bridge Running! Connect this port to FluidSynth using aconnect.")
            
            # Give FluidSynth a moment to stabilize
            time.sleep(1)
            
            # Set sound to Choir Aahs (GM Program Change 52)
            print("Setting sound to Choir Aahs (Program 52)...")
            outport.send(mido.Message('program_change', program=52))
            
            # Send it again just in case (sometimes virtual ports drop first msg)
            time.sleep(0.5)
            outport.send(mido.Message('program_change', program=52))
            
            print("Listening for serial data...")
            
            # Use mido Parser to handle running status and incomplete bytes
            parser = mido.Parser()
            choir_set = False
            
            while True:
                if ser.in_waiting:
                    # Read byte-by-byte or chunks
                    chunk = ser.read(ser.in_waiting)
                    for byte in chunk:
                        # Feed the parser
                        parser.feed_byte(byte)
                        
                        # Process available messages
                        for msg in parser:
                            if not choir_set:
                                print("Initial activity detected. Switching to Choir Aahs (Program 52)...")
                                outport.send(mido.Message('program_change', program=52))
                                choir_set = True
                                
                            print(f"Sending MIDI: {msg}")
                            outport.send(msg)
                else:
                    time.sleep(0.001)
                    
    except KeyboardInterrupt:
        print("\nStopping bridge...")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()

if __name__ == "__main__":
    run_bridge()
