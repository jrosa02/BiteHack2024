#include <Arduino.h>

//Section related to controlling both the speed and direction of rotation of the motors using
//H bridge. We assume a 5V power supply. First in case of testing we are setting defalut speed. 

//Arduino pins definition  connected to input H Bridge
int enA = 3;
int enB = 5;
int IN1 = 4; //pin connected with motor A
int IN2 = 2; //pin connected with motor A
int IN3 = 7; //pin connected with motor B
int IN4 = 6; //pin connected with motor B

//We assume that:
//left direction -> counter clockwise
//right directon -> clockwise

void goRight(){
  analogWrite(enA, 255);  
  analogWrite(enB, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
    
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void goLeft(){
  analogWrite(enA, 255); 
  analogWrite(enB, 255); 
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
      
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

}

void stop(){
  analogWrite(enA, 0); 
  analogWrite(enB, 0); 
}

void setup() {
  //Set all pins to OUTPUT mode
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

}

void loop() {
  goLeft();
  delay(2000);
  stop();
  delay(2000);
  goRight();
  delay(2000);
  stop();
  delay(2000);
  
}
