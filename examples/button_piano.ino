#include <Arduino.h>

/**
 * Single Button Piano Example
 * 
 * Hardware Requirements:
 * 1. Arduino Uno/Mega/Nano
 * 2. Push Button
 * 
 * Wiring:
 * - Button Side A  -> Arduino Pin 2
 * - Button Side B  -> Arduino GND
 * 
 * (No external resistor needed; we use the internal pull-up)
 */

// Configuration
#define BUTTON_PIN 2
#define NOTE_C4 60
#define MIDI_CHANNEL 0
#define DEBOUNCE_DELAY 50 // ms

// State
int lastButtonState = HIGH; // HIGH = Released (Pullup), LOW = Pressed
unsigned long lastDebounceTime = 0;
int buttonState = HIGH;

void sendMidi(byte cmd, byte data1, byte data2) {
  Serial.write(cmd);
  Serial.write(data1);
  Serial.write(data2);
}

void setup() {
  Serial.begin(115200);
  
  // Enable internal pull-up resistor
  // Pin will read HIGH when open, LOW when pressed (connected to GND)
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);

  // If the switch value changed, reset the debouncing timer
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    // If the state has persisted longer than the delay, we take it as the actual state
    if (reading != buttonState) {
      buttonState = reading;

      // React to state change
      if (buttonState == LOW) {
        // Button Pressed -> Note On
        sendMidi(0x90 | MIDI_CHANNEL, NOTE_C4, 127);
      } else {
        // Button Released -> Note Off
        sendMidi(0x80 | MIDI_CHANNEL, NOTE_C4, 0);
      }
    }
  }

  lastButtonState = reading;
}
