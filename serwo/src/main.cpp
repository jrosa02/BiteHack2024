#include <Arduino.h>
#include <Stepper.h>

// Defines the number of steps per rotation
const int stepsPerRevolution = 2038;
const int minSpeed = 5;
const int maxSpeed = 20;

long randSteps;
long randSpeed;

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
    //set seed, assuming pin0 is NOT connected
    randomSeed(analogRead(0));
}

void loop() {
  //generate random number of steps
  randSteps = random(-2*stepsPerRevolution, 2*stepsPerRevolution);
  // generate random speed
  randSpeed = random(minSpeed, maxSpeed);
	myStepper.setSpeed(randSpeed);
	myStepper.step(randSteps);
	delay(100);
}