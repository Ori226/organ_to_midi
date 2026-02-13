#include <Arduino.h>

/**
 * Rhythm Simulation Example
 * 
 * This sketch demonstrates how to send MIDI messages over a standard USB Serial connection.
 * It simulates a pianist playing a specific rhythm (Beethoven's 5th motif).
 * 
 * Hardware:
 *  - Arduino Uno / Mega / Nano
 *  - USB Cable connected to Computer
 * 
 * Software Stack:
 *  - Arduino: Generates timing and "Note On/Off" commands.
 *  - Serial Bridge (Python): Reads these commands from /dev/ttyACM0 and forwards them to the OS MIDI system.
 *  - FluidSynth: Receives MIDI events from the OS and generates the actual Audio (Sound).
 */

// MIDI Command Constants
#define NOTE_ON 0x90
#define NOTE_OFF 0x80

// MIDI Notes (Middle C = 60)
#define NOTE_G4 67
#define NOTE_Eb4 63

// Configuration
#define VELOCITY 127  // Max volume
#define CHANNEL 0     // MIDI Channel 1 (0-15)

// Helper function to send a 3-byte MIDI message
void sendMidi(byte cmd, byte data1, byte data2) {
  // We are sending raw bytes over Serial. 
  // The Python bridge reads these bytes and re-assembles them into MIDI messages.
  Serial.write(cmd);
  Serial.write(data1);
  Serial.write(data2);
}

void playNote(byte note, int duration) {
  sendMidi(NOTE_ON | CHANNEL, note, VELOCITY); // Press Key
  delay(duration);
  sendMidi(NOTE_OFF | CHANNEL, note, 0);       // Release Key
}

void setup() {
  // Start Serial at 115200 baud.
  // IMPORTANT: The Python bridge must match this baud rate.
  Serial.begin(115200);
}

void loop() {
  // Pattern: Short-Short-Short-Long
  // G - G - G - Eb
  
  // 3 Short notes
  for (int i = 0; i < 3; i++) {
    playNote(NOTE_G4, 150); 
    delay(50); // Silence between notes
  }
  
  // 1 Long note
  playNote(NOTE_Eb4, 1200);
  
  // Pause before repeat
  delay(2000);
}
