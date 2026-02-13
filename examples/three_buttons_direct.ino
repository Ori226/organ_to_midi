#include <Arduino.h>

/**
 * 3-Button Direct Connection Example
 * 
 * Hardware Requirements:
 * 1. Arduino Uno/Mega
 * 2. 3 Push Buttons
 * 
 * Wiring:
 * - Button 1 -> Pin 2 + GND
 * - Button 2 -> Pin 3 + GND
 * - Button 3 -> Pin 4 + GND
 * 
 * Logic:
 * - Uses Internal Pull-up Resistors (INPUT_PULLUP)
 * - Active LOW (Pressed = LOW)
 */

// Configuration
const int BUTTON_PINS[] = {2, 3, 4};
const int NUM_BUTTONS = 3;
const int BASE_NOTE = 60; // Middle C
const int MIDI_CHANNEL = 0;
const int DEBOUNCE_DELAY = 50;

// State
int lastButtonState[NUM_BUTTONS] = {HIGH, HIGH, HIGH};
int buttonState[NUM_BUTTONS] = {HIGH, HIGH, HIGH};
unsigned long lastDebounceTime[NUM_BUTTONS] = {0, 0, 0};

void sendMidi(byte cmd, byte data1, byte data2) {
  Serial.write(cmd);
  Serial.write(data1);
  Serial.write(data2);
}

void setup() {
  Serial.begin(115200);
  
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }
}

void loop() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    int reading = digitalRead(BUTTON_PINS[i]);

    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }

    if ((millis() - lastDebounceTime[i]) > DEBOUNCE_DELAY) {
      if (reading != buttonState[i]) {
        buttonState[i] = reading;

        if (buttonState[i] == LOW) {
          // Pressed -> Note On
          sendMidi(0x90 | MIDI_CHANNEL, BASE_NOTE + i, 127);
        } else {
          // Released -> Note Off
          sendMidi(0x80 | MIDI_CHANNEL, BASE_NOTE + i, 0);
        }
      }
    }

    lastButtonState[i] = reading;
  }
}
