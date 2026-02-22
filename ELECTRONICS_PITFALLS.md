# Electronics Hardware Pitfalls & Lessons

This document serves as a growing knowledge base of hard-earned lessons in electronics and hardware assembly.

## 1. The "Floating Input" Ghost
**Date:** 2026-02-21
**Symptom:** Unpredictable button presses, erratic MIDI notes, signals changing when you move your hand near the breadboard.

### The Mistake
Connecting a resistor to a data pin but leaving the other leg unconnected (or only connected to a non-conductive part of the board).

### Why it fails
Digital pins are extremely sensitive. Without a solid connection to either Ground (Pull-down) or VCC (Pull-up), the voltage on the pin "floats" between 0 and 5V. It picks up electromagnetic interference (EMI) from the environment, acting like an antenna.

### The "Pro-Tip" Fix
1. **The Bridge Rule**: A resistor is a bridge. If it doesn't cross from a "Signal" to a "Reference" (GND/VCC), it's not doing its job.
2. **Visual Continuity**: Physically trace the path. Start at the pin, go through the resistor, and ensure you land on a rail that is confirmed to be connected back to the Arduino's GND or 5V pin.
3. **The Finger Test**: If touching the wire changes the state in your code, it's definitely floating.

---
*Future lessons go here...*
