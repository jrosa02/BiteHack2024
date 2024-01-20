#include <Arduino.h>
#include <IRremote.hpp>


void setup() {
  Serial.begin(9600);
  IrReceiver.begin(12, DISABLE_LED_FEEDBACK);
  // analogWrite(10, 180);
  // TCA0.SINGLE.CTRLA = 0xFF;
}

void loop() {
  if (IrReceiver.decode()) {
    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.resume();
  }
}
