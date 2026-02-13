import time
import serial
import mido

# Configuration
SERIAL_PORT = "/dev/ttyACM0"
BAUD_RATE = 115200

def run_bridge():
    print(f"Opening Serial Port {SERIAL_PORT}...")
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
    except serial.SerialException as e:
        print(f"Error opening serial port: {e}")
        return

    print("Opening Virtual MIDI Output Port...")
    # Open virtual port (visible to ALSA)
    try:
        with mido.open_output("Arduino Bridge", virtual=True) as outport:
            print("Bridge Running! Connect this port to FluidSynth using aconnect.")
            print("Listening for serial data...")
            
            # Use mido Parser to handle running status and incomplete bytes
            parser = mido.Parser()
            
            while True:
                if ser.in_waiting:
                    # Read byte-by-byte or chunks
                    chunk = ser.read(ser.in_waiting)
                    for byte in chunk:
                        # Feed the parser
                        parser.feed_byte(byte)
                        
                        # Process available messages
                        for msg in parser:
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
