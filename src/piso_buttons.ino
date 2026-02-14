#include <Arduino.h>
#include <SPI.h>

/**
 * PISO Button Example (74HC165)
 * 
 * Hardware:
 * - 74HC165 Shift Register
 * - 3 Push Buttons connected to inputs D0, D1, D7
 * 
 * Pin Connections (as per diagram.json):
 * - LATCH (PL) -> Pin 10
 * - CLOCK (CP) -> Pin 13 (SCK)
 * - DATA (Q7)  -> Pin 12 (MISO)
 * 
 * Button Mapping:
 * - Button 1 -> D0 (Bit 0)
 * - Button 2 -> D1 (Bit 1)
 * - Button 3 -> D7 (Bit 7)
 */

// Pin Definitions
#define PIN_PL 10   // Parallel Load (Latch) -> Pin 1 of 74HC165
// SCK (Pin 13) and MISO (Pin 12) are defined by SPI library

// MIDI Configuration
#define MIDI_CHANNEL 0
#define NOTE_BASE 60 // C4

// Debug Mode: Uncomment to see binary output instead of MIDI
// #define DEBUG_MODE 

// State
byte lastState = 0;

void sendMidi(byte cmd, byte data1, byte data2) {
#ifndef DEBUG_MODE
  Serial.write(cmd);
  Serial.write(data1);
  Serial.write(data2);
#else
  Serial.print("MIDI: cmd=0x");
  Serial.print(cmd, HEX);
  Serial.print(" d1=");
  Serial.print(data1);
  Serial.print(" d2=");
  Serial.println(data2);
#endif
}

void printBinary(byte data) {
#ifdef DEBUG_MODE
  Serial.print("Shift Reg State: ");
  for (int i = 7; i >= 0; i--) {
    Serial.print((data >> i) & 1);
  }
  Serial.println();
#endif
}

void setup() {
  Serial.begin(115200);
  
  // Setup Latch Pin
  pinMode(PIN_PL, OUTPUT);
  digitalWrite(PIN_PL, HIGH);
  
  // Initialize SPI
  // 74HC165 works well with 1MHz, MSB First, Mode 0 or 2
  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0)); 
}

void loop() {
  // 1. Latch Data (Pulse Low)
  digitalWrite(PIN_PL, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_PL, HIGH);
  
  // 2. Read Byte via SPI
  // Note: The 74HC165 puts the first bit (D7 or D0 depending on wiring) on Q7 immediately.
  // Standard SPI.transfer() reads *during* the clock cycles.
  byte currentState = SPI.transfer(0);
  
  printBinary(currentState);

  // 3. Compare with Last State
  if (currentState != lastState) {
    
    // Check D0 (Button 1)
    if ((currentState & 1) != (lastState & 1)) {
       bool pressed = !(currentState & 1); // Active Low
       sendMidi(pressed ? 0x90 : 0x80, NOTE_BASE, 127);
    }

    // Check D6 (Button 2 in new diagram)
    if (((currentState >> 6) & 1) != ((lastState >> 6) & 1)) {
       bool pressed = !((currentState >> 6) & 1); // Active Low
       sendMidi(pressed ? 0x90 : 0x80, NOTE_BASE + 2, 127); // E4
    }

    // Check D7 (Button 3)
    if (((currentState >> 7) & 1) != ((lastState >> 7) & 1)) {
       bool pressed = !((currentState >> 7) & 1); // Active Low
       sendMidi(pressed ? 0x90 : 0x80, NOTE_BASE + 4, 127); // G4
    }
    
    lastState = currentState;
  }
  
  delay(10); // Simple debounce
}
