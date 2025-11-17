#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3C      // same as your working Hello World
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// 128x64 SH1106
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Fake data for now (no WiFi/API yet)
String hourStr   = "10";
String minuteStr = "25";
String ampm      = "PM";
String weekDay   = "Mon";
int    day       = 20;
String month     = "Jan";
int    year      = 2025;
int    tempC     = 26;   // fake temperature

void setup() {
  Serial.begin(115200);
  delay(100);

  if (!display.begin(i2c_Address, true)) {   // true = internal charge pump
    Serial.println(F("SH110X allocation failed. Check wiring or I2C Address."));
    for (;;); // halt
  }

  // Clear anything old
  display.clearDisplay();
  display.display();
  delay(100);
}

void loop() {
  // For demo: just use millis() to fake changing minutes
  int m = (millis() / 1000) % 60;
  if (m < 10) minuteStr = "0" + String(m);
  else        minuteStr = String(m);

  // Draw the UI
  drawWeatherScreen();

  delay(500);
}

void drawWeatherScreen() {
  display.clearDisplay();

  // ===== TIME =====
  // Big HH:MM
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(2);           // 2x size for time
  display.setCursor(0, 0);
  display.print("   ");
  display.print(hourStr);
  display.print(":");
  display.print(minuteStr);

  // AM/PM small at top-right
  display.setTextSize(1);
  display.setCursor(100, 4);
  display.print(ampm);

  // ===== DATE =====
  // Example: Mon, 20 Jan 2025
  display.setTextSize(1);
  display.setCursor(0, 22);
  display.print(weekDay);
  display.print(" ");
  display.print(day);
  display.print(" ");
  display.print(month);
  display.print(" ");
  display.print(year);

  // ===== TEMPERATURE =====
// Big temp at bottom-left
display.setTextSize(2);
display.setCursor(0, 38);
display.print(tempC);

// Save cursor position after printing the number
int16_t x = display.getCursorX();
int16_t y = display.getCursorY();

// Print the "C"
display.print(" C");

// Draw a small degree circle right before the "C"
display.drawCircle(x+2 , y + 2, 2, SH110X_WHITE);

  // (Optionally later we can draw small icons here)

  // Push buffer to the screen
  display.display();
}
