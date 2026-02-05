#ifndef MATRIX_SCANNER_H
#define MATRIX_SCANNER_H

#include <Arduino.h>
#include <MIDI.h>

class MatrixScanner {
private:
    const int* _rowPins;
    const int* _colPins;
    int _numRows;
    int _numCols;
    int _midiChannel;
    uint8_t* _keyState; // Bitmask array for key states (1 bit per key)
    unsigned long* _debounceTime; // Last time key changed state

    // Debounce delay in milliseconds
    const unsigned long DEBOUNCE_DELAY = 10; 

public:
    MatrixScanner(const int* rowPins, int numRows, const int* colPins, int numCols, int midiChannel);
    ~MatrixScanner();
    
    // Call this in the main loop to scan the matrix
    void scan();
    
    // Internal helper to handle note on/off
    void sendMIDI(int note, int velocity, bool noteOn);
};

#endif // MATRIX_SCANNER_H
