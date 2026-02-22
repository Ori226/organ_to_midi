#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/**
 * PISO Button Example (74HC165) - Daisy Chained
 * 
 * Hardware:
 * - 2x 74HC165 Shift Registers (Daisy Chained)
 */

// --- 1. Display Configuration ---
// Uncomment the mode you want to use:
#define DISPLAY_MODE_KEYBOARD  // Visual ASCII keyboard
// #define DISPLAY_MODE_BINARY    // Raw bit status (C0: 0000 0000)

// --- 2. State Configuration ---
const int NUM_CHIPS = 2; 
byte lastChipStates[NUM_CHIPS] = {0};

// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Pin Definitions
#define PIN_PL 10   // Parallel Load (Latch)

// MIDI Configuration
#define MIDI_CHANNEL 0
#define NOTE_BASE 60 // C4

void sendMidi(byte cmd, byte data1, byte data2) {
  Serial.write(cmd);
  Serial.write(data1);
  Serial.write(data2);
}

// Mode: Binary Bit Status
void updateLcdStatus_Binary(int chip, byte state) {
  lcd.setCursor(0, chip + 1);
  lcd.print("C");
  lcd.print(chip);
  lcd.print(": ");
  
  for (int i = 0; i < 8; i++) {
    bool val = (state >> i) & 1;
    lcd.print(val ? "1" : "0");
    if (i == 3) lcd.print(" ");
  }
}

// Mode: Keyboard Visualization (13 bits)
void updateLcdStatus_Keyboard(uint16_t state) {
  // Row 1: Black Keys
  lcd.setCursor(3, 1);
  lcd.print(" ");
  lcd.print((state & (1 << 1))  ? "\xFF" : "#"); // C#
  lcd.print(" ");
  lcd.print((state & (1 << 3))  ? "\xFF" : "#"); // D#
  lcd.print("  "); // E-F Gap
  lcd.print((state & (1 << 6))  ? "\xFF" : "#"); // F#
  lcd.print(" ");
  lcd.print((state & (1 << 8))  ? "\xFF" : "#"); // G#
  lcd.print(" ");
  lcd.print((state & (1 << 10)) ? "\xFF" : "#"); // A#
  lcd.print("   ");

  // Row 2: White Keys
  lcd.setCursor(3, 2);
  lcd.print((state & (1 << 0))  ? "\xFF" : "|"); // C
  lcd.print(" ");
  lcd.print((state & (1 << 2))  ? "\xFF" : "|"); // D
  lcd.print(" ");
  lcd.print((state & (1 << 4))  ? "\xFF" : "|"); // E
  lcd.print((state & (1 << 5))  ? "\xFF" : "|"); // F
  lcd.print(" ");
  lcd.print((state & (1 << 7))  ? "\xFF" : "|"); // G
  lcd.print(" ");
  lcd.print((state & (1 << 9))  ? "\xFF" : "|"); // A
  lcd.print(" ");
  lcd.print((state & (1 << 11)) ? "\xFF" : "|"); // B
  lcd.print((state & (1 << 12)) ? "\xFF" : "|"); // C (high)
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_PL, OUTPUT);
  digitalWrite(PIN_PL, HIGH);
  
  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0)); 

  lcd.init();
  lcd.backlight();
  lcd.setCursor(2, 0);
  
#ifdef DISPLAY_MODE_KEYBOARD
  lcd.print("Pedalboard C2-C3");
  updateLcdStatus_Keyboard(0);
#else
  lcd.print("Shift Reg Status");
  updateLcdStatus_Binary(0, 0x00);
  updateLcdStatus_Binary(1, 0x00);
#endif
}

void loop() {
  digitalWrite(PIN_PL, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_PL, HIGH);
  
  uint16_t combinedState = 0;
  bool changed = false;

  for (int chip = 0; chip < NUM_CHIPS; chip++) {
    byte currentState = SPI.transfer(0);
    byte previousState = lastChipStates[chip];
    combinedState |= ((uint16_t)currentState << (chip * 8));

    if (currentState != previousState) {
      changed = true;
      for (int i = 0; i < 8; i++) {
        if (((currentState >> i) & 1) != ((previousState >> i) & 1)) {
          int note = NOTE_BASE + (chip * 8) + i;
          bool pressed = (currentState >> i) & 1;
          sendMidi(pressed ? 0x90 : 0x80, note, 127);
        }
      }
      
#ifdef DISPLAY_MODE_BINARY
      updateLcdStatus_Binary(chip, currentState);
#endif
      lastChipStates[chip] = currentState;
    }
  }

#ifdef DISPLAY_MODE_KEYBOARD
  if (changed) {
    updateLcdStatus_Keyboard(combinedState);
  }
#endif
  
  delay(5); 
}