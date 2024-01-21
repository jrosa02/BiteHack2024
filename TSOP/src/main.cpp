#include <Arduino.h>
#include <IRremote.hpp>


void setup() {
  Serial.begin(9600);
  // IrReceiver.begin(12, DISABLE_LED_FEEDBACK);
  // analogWrite(10, 180);  // TCA0.SINGLE.CTRLA = 0xFF;
  pinMode(7, INPUT_PULLUP);
}

void loop() {
  // if (IrReceiver.decode()) {
  //   Serial.write("a\n");
  //   IrReceiver.resume();
  // }
  if (digitalRead(7) == 0) {
    Serial.write("aaa");
  }
}