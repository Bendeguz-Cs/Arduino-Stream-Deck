#include <Arduino.h>
#include <Rotary_encoder.h>
#include <U8g2lib.h>
#include <Wire.h>

Encoder encoder(6, 7);

// Initialize the 128x64 SSD1306 I2C OLED
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(
  U8G2_R0,
  U8X8_PIN_NONE
);

void setup() {
  u8g2.setI2CAddress(0x3C * 2);
  u8g2.begin();
  encoder.LEDRing(8, 20);
  encoder.setLimits(0, 100);   // set the range
  encoder.limits();   // enable limit enforcement
  encoder.fixedGradientBarPattern(0, 0, 30, 50, 26, 60);
  encoder.begin();
}

void loop() {
  u8g2.clearBuffer();

  // Display test text
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 10, "OLED TEST");
  u8g2.drawStr(0, 24, "SSD1306 128x64");
  u8g2.drawStr(0, 38, "I2C: 0x3C");

  u8g2.setCursor(0, 55);
  u8g2.print(encoder.read());

  // Send the image to the OLED
  u8g2.sendBuffer();

  delay(30);
}