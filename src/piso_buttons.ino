#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/**
 * PISO Button Example (74HC165) - Daisy Chained
 * 
 * Hardware:
 * - 2x 74HC165 Shift Registers (Daisy Chained)
 * 
 * Pin Connections:
 * - LATCH (PL) -> Pin 10
 * - CLOCK (CP) -> Pin 13 (SCK)
 * - DATA (Q7)  -> Pin 12 (MISO)
 */

// --- 1. State Configuration ---
const int NUM_CHIPS = 2; 
byte lastChipStates[NUM_CHIPS] = {0};

// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Pin Definitions
#define PIN_PL 10   // Parallel Load (Latch) -> Pin 1 of 74HC165

// MIDI Configuration
#define MIDI_CHANNEL 0
#define NOTE_BASE 60 // C4

// Debug Mode: Uncomment to see binary output instead of MIDI
// #define DEBUG_MODE 

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

void updateLcdStatus(int chip, byte state) {
  // Line 1 for Chip 0, Line 2 for Chip 1
  lcd.setCursor(0, chip + 1);
  lcd.print("C");
  lcd.print(chip);
  lcd.print(": ");
  
  for (int i = 0; i < 8; i++) {
    bool val = (state >> i) & 1;
    lcd.print(val ? "1" : "0");
    if (i == 3) lcd.print(" "); // Add a small space for readability
  }
}

void setup() {
  Serial.begin(115200);
  
  // Setup Latch Pin
  pinMode(PIN_PL, OUTPUT);
  digitalWrite(PIN_PL, HIGH);
  
  // Initialize SPI
  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0)); 

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Organ System");
  
  // Initialize button display states for both chips
  updateLcdStatus(0, 0x00);
  updateLcdStatus(1, 0x00);
}

void loop() {
  // 1. Latch Data (Pulse Low)
  digitalWrite(PIN_PL, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_PL, HIGH);
  
  // 2. Read all chips in the chain
  for (int chip = 0; chip < NUM_CHIPS; chip++) {
    // Read the current chip via SPI
    byte currentState = SPI.transfer(0);
    byte previousState = lastChipStates[chip];

    // 3. If something changed on this specific chip
    if (currentState != previousState) {
      
      // Iterate through all 8 bits of this chip
      for (int i = 0; i < 8; i++) {
        bool currentBit = (currentState >> i) & 1;
        bool previousBit = (previousState >> i) & 1;

        // If this specific bit changed
        if (currentBit != previousBit) {
          // Calculate MIDI Note: Base (60) + (Chip Index * 8) + Bit Index
          int note = NOTE_BASE + (chip * 8) + i;
          
          // Send Note On (0x90) if pressed, Note Off (0x80) if released
          sendMidi(currentBit ? 0x90 : 0x80, note, 127);
        }
      }
      
      // Update LCD for this specific chip
      updateLcdStatus(chip, currentState); 
      
      // Save state for next comparison
      lastChipStates[chip] = currentState;
    }
  }
  
  delay(5); // Reduced delay for better "Feel" (Latency)
}