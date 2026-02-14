#include <SPI.h>
#include <MIDI.h>

/**
 * Orla Pedalboard - Phase 1 Simulation with MIDI
 * Features: 1ms Polling, 5ms Debounce, Edge Detection
 * Output: MIDI Note On/Off messages
 */

// Create the MIDI Instance
// Using default settings (Serial, Channel 1)
MIDI_CREATE_DEFAULT_INSTANCE();

// Pin Definitions
const int LATCH_PIN = 10;
const int MISO_PIN  = 12;

// Configuration
const int numBits = 8;
const unsigned long scanInterval = 1000; /* 1ms */
const int debounceThreshold = 5;

// MIDI Settings
const int BASE_NOTE = 60; // Start at Middle C (C4)
const int VELOCITY = 127; // Max velocity

// State Tracking
unsigned long lastScanTime = 0;
uint8_t stableState = 0;
uint8_t counters[numBits] = {0};

void setup() {
  // Initialize MIDI (Calls Serial.begin(31250) implicitly)
  // To use with Hairless MIDI (Serial<->MIDI Bridge), we often need 115200.
  // But standard MIDI is 31250. Let's use standard MIDI lib init first.
  MIDI.begin(MIDI_CHANNEL_OMNI);
  
  // Setup Hardware SPI (Mode 0) for PISO reading if we were using hardware SPI,
  // but here we are bit-banging/using specific pins from previous step?
  // Wait, previous step used SPI.begin() but defined MISO_PIN as 12.
  // Let's stick to the previous robust SPI implementation.
  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  digitalWrite(LATCH_PIN, HIGH);
  
  // Debug toggle - sending generic serial text breaks MIDI parsers.
  // We should ONLY send MIDI data.
}

void loop() {
  unsigned long currentTime = micros();

  // Task Scheduler: Run scan exactly every 1000 microseconds
  if (currentTime - lastScanTime >= scanInterval) {
    lastScanTime = currentTime;

    // 1. Snapshot
    digitalWrite(LATCH_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(LATCH_PIN, HIGH);

    // 2. Capture the 'lost' bit (D7) before SPI takes over
    // Note: D7 is at Q7 immediately after latch.
    bool d7 = digitalRead(MISO_PIN);

    // 3. Get the remaining bits via SPI
    uint8_t rawData = SPI.transfer(0x00);

    // 4. Reconstruct the byte
    uint8_t currentByte = (rawData >> 1); 
    if (d7 == HIGH) currentByte |= 0x80; 

    // 5. Process each Bit for Debounce
    for (int i = 0; i < numBits; i++) {
       bool currentBit = (currentByte >> i) & 1;
       bool storedBit = (stableState >> i) & 1;

       if (currentBit != storedBit) {
          counters[i]++;
          
          if (counters[i] >= debounceThreshold) {
              // State confirmed changed
              if (currentBit == HIGH) {
                  // Switch ON -> Note ON
                  MIDI.sendNoteOn(BASE_NOTE + i, VELOCITY, 1);
                  stableState |= (1 << i);
              } else {
                  // Switch OFF -> Note OFF
                  MIDI.sendNoteOff(BASE_NOTE + i, 0, 1);
                  stableState &= ~(1 << i);
              }
              counters[i] = 0;
          }
       } else {
          counters[i] = 0;
       }
    }
  }
}