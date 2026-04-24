#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// PICO SPECIFIC PINS (Pin 9 and 10)
#define PICO_SDA 6  // GP6
#define PICO_SCL 7  // GP7

// CHANGE: We use &Wire1 instead of &Wire because GP6/GP7 are on I2C1
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET);

String hourStr   = "10";
String minuteStr = "25";
String ampm      = "PM";
String weekDay   = "Mon";
int    day       = 20;
String month     = "Jan";
int    year      = 2026; // Happy 2026!
int    tempC     = 26;

void setup() {
  Serial.begin(115200);
  
  // CRITICAL PICO STEP: Assign the pins to Wire1 before starting
  Wire1.setSDA(PICO_SDA);
  Wire1.setSCL(PICO_SCL);
  Wire1.begin();

  // Initialize the display using the SH110X library logic
  if (!display.begin(i2c_Address, true)) {
    Serial.println(F("SH110X allocation failed. Check wires on GP6/GP7."));
    for (;;);
  }

  display.clearDisplay();
  display.display();
}

void loop() {
  int m = (millis() / 1000) % 60;
  if (m < 10) minuteStr = "0" + String(m);
  else        minuteStr = String(m);

  drawWeatherScreen();
  delay(500);
}

void drawWeatherScreen() {
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  
  // TIME
  display.setTextSize(2);
  display.setCursor(20, 0);
  display.print(hourStr);
  display.print(":");
  display.print(minuteStr);

  display.setTextSize(1);
  display.setCursor(100, 4);
  display.print(ampm);

  // DATE
  display.setCursor(0, 22);
  display.print(weekDay);
  display.print(", ");
  display.print(day);
  display.print(" ");
  display.print(month);
  display.print(" ");
  display.print(year);

  // TEMP
  display.setTextSize(2);
  display.setCursor(0, 38);
  display.print(tempC);
  
  int16_t x = display.getCursorX();
  int16_t y = display.getCursorY();
  display.print(" C");
  display.drawCircle(x + 2, y + 2, 2, SH110X_WHITE);

  display.display();
}