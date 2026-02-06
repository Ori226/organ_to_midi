#include <SPI.h>

const int LATCH_PIN = 10;
const int MISO_PIN  = 12;

void setup() {
  Serial.begin(115200);
  SPI.begin();
  // Using Mode 0 (Standard)
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  digitalWrite(LATCH_PIN, HIGH);
}

void loop() {
  // 1. Snapshot
  digitalWrite(LATCH_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(LATCH_PIN, HIGH);

  // 2. Capture the 'lost' bit (D7) before SPI takes over
  bool d7 = digitalRead(MISO_PIN);

  // 3. Get the remaining bits via SPI
  uint8_t rawData = SPI.transfer(0x00);

  // 4. Reconstruct the byte
  // We take the rawData, shift it right by 1 to align it, 
  // then drop our manual D7 into the most significant bit (MSB)
  uint8_t correctedData = (rawData >> 1); 
  if (d7 == HIGH) correctedData |= 0x80; 

  // Print corrected binary
  Serial.print("Corrected Byte: ");
  for (int i = 7; i >= 0; i--) {
    Serial.print(bitRead(correctedData, i));
  }
  Serial.println();

  delay(200);
}