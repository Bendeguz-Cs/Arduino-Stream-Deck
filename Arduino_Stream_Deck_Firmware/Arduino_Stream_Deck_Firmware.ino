#include <Arduino.h>
#include <Rotary_encoder.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <USB.h>

//pin definitions
const uint8_t pinDisplaySDA  = 4;
const uint8_t pinDisplaySCL  = 5;
const uint8_t pinEncoder0CLK = 6;
const uint8_t pinEncoder0DT  = 7;
const uint8_t pinEncoder0SW  = 8;
const uint8_t pinEncoder0LED = 9;
const uint8_t pinEncoder1CLK = 10;
const uint8_t pinEncoder1DT  = 11;
const uint8_t pinEncoder1SW  = 12;
const uint8_t pinEncoder1LED = 13;
const uint8_t pinButton0     = 14;
const uint8_t pinButton1     = 15;
const uint8_t pinButton2     = 16;
const uint8_t pinButton3     = 17;
const uint8_t pinButton4     = 18;
const uint8_t pinButton5     = 19;
const uint8_t pinButton6     = 20;
const uint8_t pinButton7     = 21;

//last cycle states
uint8_t lastEncoder0val = 0;
uint8_t lastEncoder1val = 0;
bool lastEncoder0SW     = 0;
bool lastEncoder1SW     = 0;
bool lastButton0        = 0;
bool lastButton1        = 0;
bool lastButton2        = 0;
bool lastButton3        = 0;
bool lastButton4        = 0;
bool lastButton5        = 0;
bool lastButton6        = 0;
bool lastButton7        = 0;

//current cycle states
uint8_t encoder0val = 0;
uint8_t encoder1val = 0;
bool encoder0SW     = 0;
bool encoder1SW     = 0;
bool button0        = 0;
bool button1        = 0;
bool button2        = 0;
bool button3        = 0;
bool button4        = 0;
bool button5        = 0;
bool button6        = 0;
bool button7        = 0;

bool changed = true;

String timeString = "XX:XX";

Encoder encoder0(pinEncoder0CLK, pinEncoder0DT);
Encoder encoder1(pinEncoder1CLK, pinEncoder1DT);

// Initialize the 128x64 SSD1306 I2C OLED
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(
  U8G2_R0,
  U8X8_PIN_NONE
);

void setup() {
  USB.disconnect();

  USB.setProduct("PicoStreamDeck");
  //USB.setManufacturer("MyProject");
  //USB.setSerialNumber("PICO_QT_01");

  USB.connect();

  Serial.begin(115200);
  u8g2.setI2CAddress(0x3C << 1);
  u8g2.begin();
  u8g2.setDisplayRotation(U8G2_R2);

  encoder0.LEDRing(pinEncoder0LED, 20);
  encoder0.setLimits(0, 100);   // set the range
  encoder0.limits();   // enable limit enforcement
  encoder0.fixedGradientBarPattern(0, 35, 0, 40, 0, 0);
  encoder0.begin();

  encoder1.LEDRing(pinEncoder1LED, 20);
  encoder1.setLimits(0, 100);   // set the range
  encoder1.limits();   // enable limit enforcement
  encoder1.fixedGradientBarPattern(0, 35, 0, 40, 0, 0);
  encoder1.begin();

  pinMode(pinEncoder0SW, INPUT_PULLUP);
  pinMode(pinEncoder1SW, INPUT_PULLUP);

  pinMode(pinButton0, INPUT_PULLUP);
  pinMode(pinButton1, INPUT_PULLUP);
  pinMode(pinButton2, INPUT_PULLUP);
  pinMode(pinButton3, INPUT_PULLUP);
  pinMode(pinButton4, INPUT_PULLUP);
  pinMode(pinButton5, INPUT_PULLUP);
  pinMode(pinButton6, INPUT_PULLUP);
  pinMode(pinButton7, INPUT_PULLUP);
}

void loop() {
  //update last cycle values
  lastEncoder0val = encoder0val;
  lastEncoder1val = encoder1val;
  lastEncoder0SW  = encoder0SW;
  lastEncoder1SW  = encoder1SW;
  lastButton0     = button0;
  lastButton1     = button1;
  lastButton2     = button2;
  lastButton3     = button3;
  lastButton4     = button4;
  lastButton5     = button5;
  lastButton6     = button6;
  lastButton7     = button7;
 
  //update current values
  encoder0val = encoder0.read();
  encoder1val = encoder1.read();
  encoder0SW  = digitalRead(pinEncoder0SW);
  encoder1SW  = digitalRead(pinEncoder1SW);
  button0     = digitalRead(pinButton0);
  button1     = digitalRead(pinButton1);
  button2     = digitalRead(pinButton2);
  button3     = digitalRead(pinButton3);
  button4     = digitalRead(pinButton4);
  button5     = digitalRead(pinButton5);
  button6     = digitalRead(pinButton6);
  button7     = digitalRead(pinButton7);

  static bool micMute;
  static bool spkMute;

  if(encoder0val != lastEncoder0val) {
    changed = true; Serial.println(String("ENC,0,") + encoder0val);
  } else if(encoder1val != lastEncoder1val) {
    changed = true; Serial.println(String("ENC,1,") + encoder1val);
  } else if(encoder0SW != lastEncoder0SW) {
    changed = true; Serial.println(String("ESW,0,") + !encoder0SW);
    if(encoder0SW == 0) {micMute = !micMute;}
    if(micMute) {
      encoder0.fillBarPattern(15, 0, 0);
    } else {
      encoder0.fixedGradientBarPattern(0, 35, 0, 40, 0, 0);
    }
  } else if(encoder1SW != lastEncoder1SW) {
    changed = true; Serial.println(String("ESW,1,") + !encoder1SW);
    if(encoder1SW == 0) {spkMute = !spkMute;}
    if(spkMute) {
      encoder1.fillBarPattern(15, 0, 0);
    } else {
      encoder1.fixedGradientBarPattern(0, 35, 0, 40, 0, 0);
    }
  } else if(button0 != lastButton0) {
    changed = true; Serial.println(String("BTN,0,") + !button0);
  } else if(button1 != lastButton1) {
    changed = true; Serial.println(String("BTN,1,") + !button1);
  } else if(button2 != lastButton2) {
    changed = true; Serial.println(String("BTN,2,") + !button2);
  } else if(button3 != lastButton3) {
    changed = true; Serial.println(String("BTN,3,") + !button3);
  } else if(button4 != lastButton4) {
    changed = true; Serial.println(String("BTN,4,") + !button4);
  } else if(button5 != lastButton5) {
    changed = true; Serial.println(String("BTN,5,") + !button5);
  } else if(button6 != lastButton6) {
    changed = true; Serial.println(String("BTN,6,") + !button6);
  } else if(button7 != lastButton7) {
    changed = true; Serial.println(String("BTN,7,") + !button7);
  }

  if(changed) {
    changed = false;
    u8g2.clearBuffer();

    // Display test text
    u8g2.setFont(u8g2_font_t0_18_tr);

    u8g2.setCursor(0, 50);
    u8g2.print("Mic:");
    u8g2.setCursor(0, 64);
    String micValue = String(encoder0val) + "%";
    u8g2.print(micValue);

    u8g2.setCursor(60, 50);
    u8g2.print("Speaker:");
    u8g2.setCursor(92, 64);
    String spkValue = String(encoder1val) + "%";
    u8g2.print(spkValue);

    u8g2.setFont(u8g2_font_profont29_tr);

    u8g2.setCursor(25, 20);
    u8g2.print(timeString);

    // Send the image to the OLED
    u8g2.sendBuffer();
  }

  String buffer;
  if(Serial.available() > 0) {
    buffer += Serial.readStringUntil('\n');
    buffer.trim();
    Serial.println(buffer);
  }

  if(!buffer.isEmpty()) {
    int8_t commaPos = buffer.indexOf(',');
    if(commaPos != -1) {
      String messageType    = buffer.substring(0, commaPos);
      String messageContent = buffer.substring(commaPos + 1);

      //process message
      if(messageType == "SMUTE") {
        if(messageContent == "1") {
          encoder1.fillBarPattern(15, 0, 0);
          spkMute = true;
        } else if(messageContent == "0") {
          encoder1.fixedGradientBarPattern(0, 35, 0, 40, 0, 0);
          spkMute = false;
        }
      } else if(messageType == "MMUTE") {
        if(messageContent == "1") {
          encoder0.fillBarPattern(15, 0, 0);
          micMute = true;
        } else if(messageContent == "0") {
          encoder0.fixedGradientBarPattern(0, 35, 0, 40, 0, 0);
          micMute = false;
        }
      } else if(messageType == "SVOL") {
        encoder1.setPosition(messageContent.toInt());
        encoder1.updatePattern();
      } else if(messageType == "MVOL") {
        encoder0.setPosition(messageContent.toInt());
        encoder0.updatePattern();
      } else if(messageType == "TIME") {
        //update stored time
        Serial.println(messageContent);
        timeString = messageContent;
        changed = true;
      }
    } else {
      //invalid message
    }
  }

  delay(20);
}