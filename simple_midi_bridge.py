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
    try:
        with mido.open_output("Arduino Bridge", virtual=True) as outport:
            print("Bridge Running!")
            
            # Use mido Parser
            parser = mido.Parser()
            prog_set = False
            
            # Helper to set bank and program
            def set_patch(bank, program):
                print(f"Setting Bank {bank}, Program {program}...")
                # Bank Select MSB (CC 0)
                outport.send(mido.Message('control_change', control=0, value=bank))
                # Bank Select LSB (CC 32)
                outport.send(mido.Message('control_change', control=32, value=0))
                # Program Change
                outport.send(mido.Message('program_change', program=program))

            # Initial setup for Theremin (Bank 1, Program 1)
            set_patch(1, 1)
            
            while True:
                if ser.in_waiting:
                    chunk = ser.read(ser.in_waiting)
                    for byte in chunk:
                        parser.feed_byte(byte)
                        for msg in parser:
                            if not prog_set:
                                # Ensure patch is set on first activity
                                set_patch(1, 1)
                                prog_set = True
                            outport.send(msg)
                else:
                    time.sleep(0.001)
                    
    except KeyboardInterrupt:
        print("\nStopping bridge...")
    finally:
        if 'ser' in locals() and ser is not None and ser.is_open:
            ser.close()

if __name__ == "__main__":
    run_bridge()
