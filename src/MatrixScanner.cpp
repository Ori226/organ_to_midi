#include "MatrixScanner.h"

// Initialize MIDI instance. 
// Note: In a real multi-manual setup, we might want to pass a reference to a MIDI interface,
// strictly speaking, but using the global MIDI object is common in Arduino sketches. 
// If using USB-MIDI on Teensy, the 'MIDI' object is automatically available.
// If using standard Serial MIDI, we usually need to instantiate it.
// For this implementation, we will assume standard MIDI library usage.

#ifdef USER_TEENSY
  // Teensy USB MIDI
#else
  // Standard Serial MIDI setup (typically on Serial1 for Mega)
  // If not defined elsewhere, specific instantiation might be needed in main
  // But code inside a class usually references the global or passed instance.
  // We'll use the standard MIDI namespace methods which often wrap the default instance.
  // We need to declare the global MIDI object to use it here.
  extern midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MIDI;
#endif

// We need a way to reference the main MIDI object. 
// A clean way is to declare it extern or use the standard `MIDI` object if available.
// For flexibility, let's assume `MIDI` is globally available (standard library behavior).
// If using HardwareSerial, `MIDI_CREATE_DEFAULT_INSTANCE();` in main handles this.

MatrixScanner::MatrixScanner(const int* rowPins, int numRows, const int* colPins, int numCols, int midiChannel) 
    : _rowPins(rowPins), _numRows(numRows), _colPins(colPins), _numCols(numCols), _midiChannel(midiChannel) {
    
    int totalKeys = _numRows * _numCols;
    _keyState = new uint8_t[totalKeys];
    _debounceTime = new unsigned long[totalKeys];
    
    // Initialize state
    for (int i = 0; i < totalKeys; i++) {
        _keyState[i] = 0; // 0 = released
        _debounceTime[i] = 0;
    }
    
    // Setup pins
    // Rows as Outputs (Active Low scanning usually implies driving one row LOW and checking Cols)
    // Coiumns as Inputs with Pullup
    
    for (int i = 0; i < _numRows; i++) {
        pinMode(_rowPins[i], OUTPUT);
        digitalWrite(_rowPins[i], HIGH); // Idle High
    }
    
    for (int i = 0; i < _numCols; i++) {
        pinMode(_colPins[i], INPUT_PULLUP);
    }
}

MatrixScanner::~MatrixScanner() {
    delete[] _keyState;
    delete[] _debounceTime;
}

void MatrixScanner::scan() {
    for (int r = 0; r < _numRows; r++) {
        // Activate Row (Active LOW)
        digitalWrite(_rowPins[r], LOW);
        
        // Allow signal to settle? (Micros wait might be needed for long cables/capacitance)
        // delayMicroseconds(20); 

        for (int c = 0; c < _numCols; c++) {
            // Read Column
            // LOW means pressed (because Input Pullup + Row LOW)
            bool isPressed = (digitalRead(_colPins[c]) == LOW);
            int keyIndex = (r * _numCols) + c;
            
            // Check changes
            bool lastState = _keyState[keyIndex];
            
            if (isPressed != lastState) {
                // Debounce check
                if ((millis() - _debounceTime[keyIndex]) > DEBOUNCE_DELAY) {
                    // Valid change
                    _keyState[keyIndex] = isPressed;
                    _debounceTime[keyIndex] = millis();
                    
                    // MIDI Note mapping
                    // Basic linear mapping: Note = keyIndex + Offset.
                    // We need a base note. Typically C2 (36) or similar.
                    // Let's assume a start note of 36 for now. 
                    // Ideally pass this in constructor.
                    int note = 36 + keyIndex; 
                    
                    if (isPressed) {
                        sendMIDI(note, 127, true);
                    } else {
                        sendMIDI(note, 0, false);
                    }
                }
            }
        }
        
        // Deactivate Row
        digitalWrite(_rowPins[r], HIGH);
    }
}

void MatrixScanner::sendMIDI(int note, int velocity, bool noteOn) {
    // Basic MIDI sending wrapper
    // Using the standard Arduino MIDI Library syntax
    #ifdef USER_TEENSY
       // Teensy USB MIDI
       if (noteOn) {
           usbMIDI.sendNoteOn(note, velocity, _midiChannel);
       } else {
           usbMIDI.sendNoteOff(note, velocity, _midiChannel);
       }
    #else
       // Standard Serial MIDI 
       // We assume the use of the standard MIDI library which creates a global 'MIDI' instance
       // when using MIDI_CREATE_DEFAULT_INSTANCE() in the main sketch.
       if (noteOn) {
           MIDI.sendNoteOn(note, velocity, _midiChannel);
       } else {
           MIDI.sendNoteOff(note, velocity, _midiChannel);
       }
    #endif 
}
