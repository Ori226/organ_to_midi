#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TESTING LCD...");
  lcd.setCursor(0, 1);
  lcd.print("Address: 0x27");
  lcd.setCursor(0, 2);
  lcd.print("If you see this");
  lcd.setCursor(0, 3);
  lcd.print("It works!");
}

void loop() {
  lcd.backlight();
  delay(500);
  lcd.noBacklight();
  delay(500);
}
