#include <Arduino.h>
#include <Rotary_encoder.h>
#include <U8g2lib.h>
#include <Wire.h>

const uint8_t displaySDA  = 4;
const uint8_t displaySCL  = 5;
const uint8_t encoder0CLK = 6;
const uint8_t encoder0DT  = 7;
const uint8_t encoder0SW  = 8;
const uint8_t encoder0LED = 9;
const uint8_t encoder1CLK = 10;
const uint8_t encoder1DT  = 11;
const uint8_t encoder1SW  = 12;
const uint8_t encoder1LED = 13;
const uint8_t button0     = 14;
const uint8_t button1     = 15;
const uint8_t button2     = 16;
const uint8_t button3     = 17;
const uint8_t button4     = 18;
const uint8_t button5     = 19;
const uint8_t button6     = 20;
const uint8_t button7     = 21;

Encoder encoder0(encoder0CLK, encoder0DT);
Encoder encoder1(encoder1CLK, encoder1DT);

// Initialize the 128x64 SSD1306 I2C OLED
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(
  U8G2_R0,
  U8X8_PIN_NONE
);

void setup() {
  Serial.begin(9600);
  u8g2.setI2CAddress(0x3C << 1);
  u8g2.begin();
  u8g2.setDisplayRotation(U8G2_R2);

  encoder0.LEDRing(encoder0LED, 20);
  encoder0.setLimits(0, 100);   // set the range
  encoder0.limits();   // enable limit enforcement
  encoder0.fixedGradientBarPattern(0, 35, 0, 40, 0, 0);
  encoder0.begin();

  encoder1.LEDRing(encoder1LED, 20);
  encoder1.setLimits(0, 100);   // set the range
  encoder1.limits();   // enable limit enforcement
  encoder1.fixedGradientBarPattern(0, 35, 0, 40, 0, 0);
  encoder1.begin();

  pinMode(encoder0SW, INPUT_PULLUP);
  pinMode(encoder1SW, INPUT_PULLUP);

  pinMode(button0, INPUT_PULLUP);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  pinMode(button5, INPUT_PULLUP);
  pinMode(button6, INPUT_PULLUP);
  pinMode(button7, INPUT_PULLUP);
}

void loop() {
  u8g2.clearBuffer();

  // Display test text
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 10, "OLED TEST");
  u8g2.drawStr(0, 24, "SSD1306 128x64");
  u8g2.drawStr(0, 38, "I2C: 0x3C");

  u8g2.setCursor(0, 55);
  u8g2.print(encoder0.read());

  // Send the image to the OLED
  u8g2.sendBuffer();

  Serial.print(encoder0.read());
  Serial.print(", ");
  Serial.print(encoder1.read());
  Serial.print(", ");
  Serial.print(digitalRead(encoder0SW));
  Serial.print(", ");
  Serial.print(digitalRead(encoder1SW));
  Serial.print(", ");
  Serial.print(digitalRead(button0));
  Serial.print(", ");
  Serial.print(digitalRead(button1));
  Serial.print(", ");
  Serial.print(digitalRead(button2));
  Serial.print(", ");
  Serial.print(digitalRead(button3));
  Serial.print(", ");
  Serial.print(digitalRead(button4));
  Serial.print(", ");
  Serial.print(digitalRead(button5));
  Serial.print(", ");
  Serial.print(digitalRead(button6));
  Serial.print(", ");
  Serial.println(digitalRead(button7));

  delay(30);
}