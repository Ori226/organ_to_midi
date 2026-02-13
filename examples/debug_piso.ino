#include <Arduino.h>
#include <SPI.h>

/**
 * PISO Hardware Debugger
 * 
 * Purpose: Verify 74HC165 Shift Register and Button connections.
 * 
 * Hardware:
 * - LATCH (PL) -> Pin 10
 * - CLOCK (CP) -> Pin 13
 * - DATA (Q7)  -> Pin 12
 * 
 * Output: 
 * Prints the raw byte in Binary (0-1) to Serial Monitor.
 */

#define PIN_PL 10

void setup() {
  Serial.begin(115200);
  Serial.println("PISO Debugger Started");
  
  pinMode(PIN_PL, OUTPUT);
  digitalWrite(PIN_PL, HIGH);
  
  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
}

void loop() {
  // Latch
  digitalWrite(PIN_PL, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_PL, HIGH);
  
  // Read
  byte val = SPI.transfer(0);
  
  // Print binary representation
  Serial.print("Raw Value: ");
  for (int i = 7; i >= 0; i--) {
    Serial.print((val >> i) & 1);
  }
  Serial.print(" (Hex: 0x");
  Serial.print(val, HEX);
  Serial.println(")");
  
  delay(200); // Slow update for readability
}
