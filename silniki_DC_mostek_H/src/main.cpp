#include <Arduino.h>

//Section related to controlling both the speed and direction of rotation of the motors using
//H bridge. We assume a 5V power supply. First in case of testing we are setting defalut speed. 

//Arduino pins definition  connected to input H Bridge
#define enA 3
#define enB 5
#define IN1 4 //pin connected with motor A
#define IN2 2 //pin connected with motor A
#define IN3 7 //pin connected with motor B
#define IN4 6 //pin connected with motor B

//pins for proximity sensor
#define trigPin 12 //proximity sensor trigger pin
#define echoPin 13 //proximity sensor echo pin

uint8_t proxSensorLeft = 0;
uint8_t proxSensorRight = 0;
long currentMillis = 0;
long prevMillis = 0;
uint8_t chooseDirection = 3;
long randTime = 0;


void setup() {
  //Set all pins to OUTPUT mode
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  randomSeed(analogRead(0));
}

void loop() {
  chooseDirection = random(3);
  randTime = random(2000);
  switch (chooseDirection) {
    case 0:
      stop(randTime);
      break;
    case 1:
      if (!proxSensorLeft) {
        goLeft(randTime);
      }
      break;
    case 2:
      if (!proxSensorRight) {
        goRight(randTime);
      }
      break;
    default;
      stop();
  }
}

//We assume that:
//left direction -> counter clockwise
//right directon -> clockwise

//helper functions definitions

void stop(uint8_t t=100){
  currentMillis = millis();
  analogWrite(enA, 0); 
  analogWrite(enB, 0);
  while(currentMillis - prevMillis >= t) {
    currentMillis = millis();
  }
  prevMillis = currentMillis;
}

void goRight(uint8_t t){
  currentMillis = millis();
  if (!proxSensorRight) {
    analogWrite(enA, 255);  
    analogWrite(enB, 255);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  
  while(currentMillis - prevMillis >= t && !proxSensorRight) {
    currentMillis = millis();
  }
  prevMillis = currentMillis;
  stop();
}

void goLeft(uint8_t t){
  currentMillis = millis();
  if (!proxSensorLeft) {
    analogWrite(enA, 255); 
    analogWrite(enB, 255); 
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  while(currentMillis - prevMillis >= t && !proxSensorLeft) {
    currentMillis = millis();
  }
  prevMillis = currentMillis;
  stop();
}