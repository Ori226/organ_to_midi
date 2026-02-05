#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// --- System Configuration ---
// Uncomment the following line if using Teensy 4.1 to enable specific optimizations
// #define USER_TEENSY

// --- MIDI Configuration ---
#define MIDI_CHANNEL_UPPER  1
#define MIDI_CHANNEL_LOWER  2
#define MIDI_CHANNEL_PEDAL  3

// --- Matrix Configuration ---
// These are placeholder values. Update with actual wiring.

// Upper Manual Dimensions
#define UPPER_ROWS 8
#define UPPER_COLS 8

// Lower Manual Dimensions
#define LOWER_ROWS 8
#define LOWER_COLS 8

// Pedalboard Dimensions
#define PEDAL_ROWS 4
#define PEDAL_COLS 8

// --- Pin Definitions (Placeholder) ---
// Upper Manual Pins
const int UPPER_ROW_PINS[UPPER_ROWS] = {22, 23, 24, 25, 26, 27, 28, 29};
const int UPPER_COL_PINS[UPPER_COLS] = {30, 31, 32, 33, 34, 35, 36, 37};

// Lower Manual Pins
const int LOWER_ROW_PINS[LOWER_ROWS] = {38, 39, 40, 41, 42, 43, 44, 45};
const int LOWER_COL_PINS[LOWER_COLS] = {46, 47, 48, 49, 50, 51, 52, 53};

// Pedal Pins
const int PEDAL_ROW_PINS[PEDAL_ROWS] = {54, 55, 56, 57}; // A0-A3
const int PEDAL_COL_PINS[PEDAL_COLS] = {58, 59, 60, 61, 62, 63, 64, 65}; // A4-A11

#endif // CONFIG_H
