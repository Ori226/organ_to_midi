# Electronics Hardware Pitfalls & Lessons

This document serves as a growing knowledge base of hard-earned lessons in electronics and hardware assembly.

## Hardware Pitfalls

| Problem | Symptom | Fix |
| :--- | :--- | :--- |
| **Floating Inputs** | LCD shows random "ON/OFF" flickering; MIDI notes play themselves. | Resistors MUST connect between the Data Pin and 5V (Pull-up) or Ground (Pull-down). |
| **Daisy Chain Order** | Second register data is missing or shifted. | Ensure `SER_IN` of Chip A connects to `Q7` of Chip B. Check parallel Clocks. |

## Software & MIDI Pitfalls

| Problem | Symptom | Fix |
| :--- | :--- | :--- |
| **Missing Dependencies** | MIDI bridge fails to start with `ModuleNotFoundError`. | Install `pyserial`, `mido`, and `python-rtmidi` in the environment. |
| **GitHub LFS/HTML Trap** | Soundfont file exists but is invalid (HTML/Text). | Use **Raw** links for `curl` and verify with `file choir.sf2`. |
| **MIDI Timing Race** | Program Change (Choir) is ignored at startup. | Virtual ports take time to "latch". Fix: Send the command on the **first note activity**. |

---
*Last Updated: 2026-02-22*
