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
#define trigPinLeft 12 //proximity sensor trigger pin
#define trigPinRight 14
#define echoPinLeft 13 //proximity sensor echo pin
#define echoPinRight 15

volatile uint8_t proxSensorLeft = 0;
volatile uint8_t proxSensorRight = 0;

long currentMillis = 0;
long prevMillis = 0;
uint8_t chooseDirection = 3;
long randTime = 0;


//helper function for checking distance
void checkDistance(int trigPin, int echoPin) {
  proxSensorLeft = 0;
  proxSensorRight = 0;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);

  long time = pulseIn(echoPin, HIGH);
  long dist = time/58;

  if (dist < 5) {
    if (trigPin == trigPinLeft) {
      proxSensorLeft = 1;
    }
    else if (trigPin == trigPinRight) {
      proxSensorRight = 1;
    }
  }
}

//helper functions definitions
//We assume that:
//left direction -> counter clockwise
//right directon -> clockwise

void stop(uint8_t t=100){
  currentMillis = millis();
  analogWrite(enA, 0); 
  analogWrite(enB, 0);
  while(currentMillis - prevMillis <= t) {
    currentMillis = millis();
  }
  prevMillis = currentMillis;
}

void goRight(uint8_t t){
  currentMillis = millis();
  checkDistance(trigPinRight, echoPinRight);
  if (!proxSensorRight) {
    analogWrite(enA, 255);  
    analogWrite(enB, 255);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  
  while(currentMillis - prevMillis <= t && !proxSensorRight) {
    checkDistance(trigPinRight, echoPinRight);
    currentMillis = millis();
  }
  prevMillis = currentMillis;
  stop();
}

void goLeft(uint8_t t){
  currentMillis = millis();
  checkDistance(trigPinLeft, echoPinLeft);
  if (!proxSensorLeft) {
    analogWrite(enA, 255); 
    analogWrite(enB, 255); 
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  while(currentMillis - prevMillis <= t && !proxSensorLeft) {
    checkDistance(trigPinLeft, echoPinLeft);
    currentMillis = millis();
  }
  prevMillis = currentMillis;
  stop();
}


void setup() {
  //Set all pins to OUTPUT mode
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(trigPinRight, OUTPUT);
  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinRight, INPUT);
  pinMode(echoPinLeft, INPUT);
  randomSeed(analogRead(0));
  delay(10000);
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
    default:
      stop();
  }
}


