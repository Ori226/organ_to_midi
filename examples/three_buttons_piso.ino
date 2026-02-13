#include <Arduino.h>
#include <SPI.h>

/**
 * 3-Button Shift Register Example (PISO)
 * 
 * Hardware:
 * - 74HC165 Shift Register
 * - 3 Push Buttons connected to inputs D0, D1, D2
 * 
 * Pin Connections (as per diagram.json):
 * - LATCH (PL) -> Pin 9
 * - CLOCK (CP) -> Pin 13 (SCK)
 * - DATA (Q7)  -> Pin 12 (MISO)
 * 
 * Logic:
 * - We read 1 byte (8 bits) from the shift register.
 * - Bit 0 = Button 1
 * - Bit 1 = Button 2
 * - Bit 2 = Button 3
 */

// Pin Definitions
#define PIN_PL 9   // Parallel Load (Latch) -> Pin 1 of 74HC165
// SCK (Pin 13) and MISO (Pin 12) are defined by SPI library

// MIDI Configuration
#define MIDI_CHANNEL 0
#define NOTE_BASE 60 // C4

// State
byte lastState = 0;

void sendMidi(byte cmd, byte data1, byte data2) {
  Serial.write(cmd);
  Serial.write(data1);
  Serial.write(data2);
}

void setup() {
  Serial.begin(115200);
  
  // Setup Latch Pin
  pinMode(PIN_PL, OUTPUT);
  digitalWrite(PIN_PL, HIGH);
  
  // Initialize SPI
  // 74HC165 works well with 1MHz, MSB First, Mode 0 or 2
  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0)); // reduced speed for safety
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
  
  // 3. Compare with Last State
  if (currentState != lastState) {
    for (int i = 0; i < 3; i++) {
        // Check if bit 'i' changed
        bool currentBit = (currentState >> i) & 1;
        bool lastBit = (lastState >> i) & 1;
        
        if (currentBit != lastBit) {
            // Logic: High = Pressed? Low = Pressed?
            // Depends on wiring (Pull-up vs Pull-down).
            // Usually Shift Registers with Buttons pull inputs HIGH or LOW.
            // Let's assume Active High for now (based on Wokwi defaults often being VCC->Btn->Input + Pulldown)
            // BUT diagram.json shows PISO:VCC -> Btn -> D0. And usually inputs need pulldowns.
            // If the Wokwi PISO has internal pull-ups/downs, we'll see.
            // Standard assumption: Change detected.
            
            if (currentBit == HIGH) {
                // Note On
                sendMidi(0x90 | MIDI_CHANNEL, NOTE_BASE + i, 127);
            } else {
                // Note Off
                sendMidi(0x80 | MIDI_CHANNEL, NOTE_BASE + i, 0);
            }
        }
    }
    lastState = currentState;
  }
  
  delay(10); // Simple debounce / rate limit
}
