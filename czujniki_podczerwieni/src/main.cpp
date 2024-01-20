#include <Arduino.h>
#include <IRremote.hpp>


void setup() {
  Serial.begin(9600);
  IrReceiver.begin(12, DISABLE_LED_FEEDBACK);
}

void loop() {
  if (IrReceiver.decode()) {
    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.resume();
  }
}
