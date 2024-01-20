#include <Arduino.h>

const int TriggerPin = 2;
uint8_t triggerPulled = 0;
uint32_t delayCounter = 0;

void setup() {
  Serial.begin(9600);
  pinMode(TriggerPin, INPUT_PULLUP);
      analogWrite(10, 180);
    TCA0.SINGLE.CTRLA = 0xFF;
}

void loop() {
  if (digitalRead(TriggerPin) == 0 && triggerPulled == 0) {
    triggerPulled = 1;
    delayCounter = 0;
    analogWrite(10, 180);
    TCA0.SINGLE.CTRLA = 0xFF;
    Serial.write("Trigger pulled\n");
  }

  delayCounter = delayCounter + 1;
  if (delayCounter >= 50 && delayCounter <= 60) {
    analogWrite(10, 0);
  }

  if (delayCounter >= 200000) {
    delayCounter = 0;
    triggerPulled = 0;
  }
}