#include <Arduino.h>
#include <Stepper.h>
#include <stdint.h>

//Section related to controlling both the speed and direction of rotation of the motors using
//H bridge. We assume a 5V power supply. First in case of testing we are setting defalut speed. 

//Arduino pins definition  connected to input H Bridge
#define enA 3
#define enB 5
#define IN1 4 //pin connected with motor A
#define IN2 12 //pin connected with motor A
#define IN3 7 //pin connected with motor B
#define IN4 6 //pin connected with motor B

#define RECEIVER 13 //pin used for input from TSOP

#define STOPTIME 2000
#define COMMAND_LENGHT 44

//stepper pins
#define STEPPERIN1 8
#define STEPPERIN2 10
#define STEPPERIN3 9
#define STEPPERIN4 11

//stepper
const int stepsPerRevolution = 2038;

const int32_t command_table[] = {1000, -2000, 0, 1000, 0, 1500, -1000, 0,  -1000, 2000, -1000, 0, 1000, 0, -1000, 0, -1000, 1000, 0,\
                                1000, 0, -2000, 0, 1000, 1500, 0, -2000, 1000, 0 -1000, 1500, -1000, 2000, -1000, 0, 1000, 0, -1000,\
                                 0, -1000, 1000, 0, 1000, -2000, 0};

bool end_flag;
bool received;

long stepCounter = 0;
int8_t steps = 20;

Stepper myStepper = Stepper(stepsPerRevolution, STEPPERIN1, STEPPERIN2, STEPPERIN3, STEPPERIN4);

//helper functions definitions
//We assume that:
//left direction -> counter clockwise
//right directon -> clockwise

void stop(void){
  analogWrite(enA, 0); 
  analogWrite(enB, 0);
  //Serial.write("stop\n");
}

void goRight(void){
    analogWrite(enA, 255);  
    analogWrite(enB, 255);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    //Serial.write("right\n");
}

void goLeft(void){
    analogWrite(enA, 255); 
    analogWrite(enB, 255); 
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    //Serial.write("left\n");
}

int8_t decodeCommand(int32_t command)
{
  if (command < 0)
  {
    return -1;
  }
  else if (command > 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int8_t getCommand(uint32_t time)
{
  static uint8_t command_index = 0;
  static int32_t command_sum = 0;
  static int8_t ret_command = 0;

  // Serial.write("\n");
  // Serial.print(command_index);
  // Serial.write("\n");
  // Serial.print(time);
  // Serial.write("\n");
  // Serial.print(int32_t((int64_t)time - (command_sum + abs(command_table[command_index]))));
  // Serial.write("\n");  

  if((int64_t)time - (command_sum + abs(command_table[command_index])) > 0)
  {//choose next move
    command_sum += abs(command_table[command_index]);
    ++command_index;
    ret_command = decodeCommand(command_table[command_index]);

    if (0 == ret_command) // implementing stop_time
    {
      command_sum += STOPTIME;
    }
    
    //Serial.write("Getting new command");
  }
  else
  {
    //Serial.write("Old command");
  }

    if (command_index >= COMMAND_LENGHT)
  {
    end_flag = true;
  }

  return ret_command;
}

void chooseDirection(void)
{

  uint32_t current_time = millis();
  int8_t command = getCommand(current_time);

  if (command < 0)
  {
    goRight();
  }
  else if (command > 0)
  {
    goLeft();
  }
  else
  {
    stop();
  } 

  //Serial.write("Setting direction \n\n");
}

void flashLEDs() {
  //turn red LEDs on, wait for two seconds, turn them off
  stop();
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  Serial.write("received!\n");
  delay(1000);
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
}

void setup() {
  //Set all pins to OUTPUT mode
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(RECEIVER, INPUT_PULLUP);
  Serial.begin(9600);

  myStepper.setSpeed(10);

  end_flag = false;
  received = false;
}

void loop() {
  //left-right movement 
  if (false == end_flag)
  {
    chooseDirection();
    //receiver
    if (digitalRead(RECEIVER) == 0) {
      received = true;
    }

    if (received) {
      flashLEDs();
      received = false;
    }
    
    myStepper.step(steps);
    stepCounter += abs(steps);
    if (stepCounter >= stepsPerRevolution) {
      stepCounter = 0;
      steps *= -1;
    }

  }
  else
  {
    Serial.print("END\nEND\nEND");
  }
  
}


