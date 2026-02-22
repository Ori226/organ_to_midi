#!/bin/bash

# Configuration
SKETCH="src/piso_buttons.ino"
BUILD_DIR="build"
CLI="./bin/arduino-cli"
FQBN="arduino:avr:uno"

echo "Using Arduino CLI at $CLI"
echo "Compiling $SKETCH for $FQBN..."

# 1. Create temporary directory for compilation (Arduino CLI requirement)
TEMP_SKETCH_DIR="build_temp/piso_buttons"
mkdir -p "$TEMP_SKETCH_DIR"
cp "$SKETCH" "$TEMP_SKETCH_DIR/piso_buttons.ino"

# Clear previous build artifacts to avoid ambiguity during upload
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

# 2. Compile
$CLI compile --fqbn "$FQBN" "$TEMP_SKETCH_DIR" --output-dir "$BUILD_DIR"

if [ $? -eq 0 ]; then
    echo "----------------------------------------"
    echo "SUCCESS: Build finished."
    echo "Binary: $BUILD_DIR/piso_buttons.ino.hex"
    echo "----------------------------------------"
    
    # 3. Auto-Detect Port and Upload
    echo "Searching for Arduino Uno..."
    PORT=$($CLI board list | grep "Arduino UNO" | awk '{print $1}')
    
    if [ -n "$PORT" ]; then
        echo "Found Arduino on $PORT. Uploading..."
        $CLI upload -p "$PORT" --fqbn "$FQBN" "$TEMP_SKETCH_DIR" --input-dir "$BUILD_DIR"
        
        if [ $? -eq 0 ]; then
            echo "----------------------------------------"
            echo "SUCCESS: Upload complete on $PORT."
            echo "----------------------------------------"
        else
            echo "----------------------------------------"
            echo "ERROR: Upload failed."
            echo "----------------------------------------"
        fi
    else
        echo "----------------------------------------"
        echo "WARNING: No Arduino UNO detected. Skipping upload."
        echo "----------------------------------------"
    fi
else
    echo "----------------------------------------"
    echo "ERROR: Compilation failed."
    echo "----------------------------------------"
    exit 1
fi
