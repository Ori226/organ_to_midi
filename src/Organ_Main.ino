/**
 * Orla Pedalboard - Phase 1 Simulation
 * Features: 1ms Polling, 5ms Debounce, Edge Detection
 * Based on Uri Shaked's Wiring (Data:2, Clock:3, Latch:4)
 */

// Pin Definitions based on your provided JSON
const int dataPin  = 2;   /* Q7 Serial Output */
const int clockPin = 3;   /* CP Clock Pulse   */
const int latchPin = 4;   /* PL Parallel Load */

// Configuration
const int numBits = 8;               /* Number of inputs to scan */
const unsigned long scanInterval = 1000; /* 1ms in microseconds */
const int debounceThreshold = 5;     /* Required stable samples for 5ms debounce */

// State Tracking
unsigned long lastScanTime = 0;
uint8_t stableState = 0;             /* Current debounced state of all 8 bits */
uint8_t counters[numBits] = {0};     /* Stability counters for each input */

void setup() {
  Serial.begin(115200);
  pinMode(dataPin, INPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  
  digitalWrite(latchPin, HIGH);
  Serial.println("System Ready: English Logic / 5ms Debounce / Polled at 1kHz");
}

void loop() {
  unsigned long currentTime = micros();

  // Task Scheduler: Run scan exactly every 1000 microseconds
  if (currentTime - lastScanTime >= scanInterval) {
    lastScanTime = currentTime;

    // Step 1: Capture Parallel Inputs (Snapshot)
    digitalWrite(latchPin, LOW);
    digitalWrite(latchPin, HIGH);

    // Step 2: Shift and Process each Bit
    for (int i = 0; i < numBits; i++) {
      int currentBit = digitalRead(dataPin);

      // --- Debounce & Edge Detection Logic ---
      
      // Determine if this bit differs from our current stable state
      bool isDifferent = (currentBit != ((stableState >> i) & 1));

      if (isDifferent) {
        counters[i]++; // Increment counter if state is transitioning/noisy
        
        if (counters[i] >= debounceThreshold) {
          // Threshold reached: Confirm the state change
          if (currentBit == HIGH) {
            handleEdge(i, true);  // Rising Edge
            stableState |= (1 << i);
          } else {
            handleEdge(i, false); // Falling Edge
            stableState &= ~(1 << i);
          }
          counters[i] = 0; // Reset counter after confirmation
        }
      } else {
        // Signal matches stable state, reset counter to suppress noise
        counters[i] = 0;
      }

      // Pulse clock to shift in the next bit for the next iteration
      digitalWrite(clockPin, HIGH);
      digitalWrite(clockPin, LOW);
    }
  }
}

/**
 * Triggered only once per state change (On Press / On Release)
 */
void handleEdge(int bitIndex, bool state) {
  if (state) {
    Serial.print("Bit "); Serial.print(bitIndex); Serial.println(": HIGH [Logic Change]");
  } else {
    Serial.print("Bit "); Serial.print(bitIndex); Serial.println(": LOW [Logic Change]");
  }
}